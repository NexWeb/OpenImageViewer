#pragma warning(disable : 4275 ) // disables warning 4275, pop and push from exceptions
#pragma warning(disable : 4251 ) // disables warning 4251, the annoying warning which isn't needed here...
#include "precompiled.h"
#include "oiv.h"
#include "Plugins/PluginJpeg.h"
#include "Plugins/PluginPng.h"
#include "Plugins/PluginFreeImage.h"
#include "External\easyexif\exif.h"
#include "Interfaces\IRenderer.h"
#include "NullRenderer.h"
#include "Image/ImageUtil.h"
#include "Configuration.h"

#if OIV_BUILD_PLUGIN_PSD == 1
#include <Codecs/CodecPSD/Include/CodecPSDFactory.h>
#endif


#if OIV_BUILD_RENDERER_D3D11 == 1
#include "../OIVD3D11Renderer/OIVD3D11RendererFactory.h"
#endif


#if OIV_BUILD_RENDERER_GL == 1
#include "../OIVGLRenderer/OIVGLRendererFactory.h"
#endif


namespace OIV
{
    OIV::OIV() :
          fScrollState(this)
        , fIsRefresing(false)
        , fParent(0)
        , fFilterLevel(2)
        , fShowGrid(false)
        , fClientWidth(-1)
        , fClientHeight(-1)

    {
#if OIV_BUILD_PLUGIN_JPEG == 1
        fImageLoader.InstallPlugin(new PluginJpeg());
#endif
#if OIV_BUILD_PLUGIN_PNG == 1
        fImageLoader.InstallPlugin(new PluginPng());
#endif
#if OIV_BUILD_PLUGIN_PSD == 1
        fImageLoader.InstallPlugin(CodecPSDFactory::Create());
#endif
        

        //TODO: remove free image codec with specialized codecs.
#if OIV_BUILD_PLUGIN_FREE_IMAGE == 1
        fImageLoader.InstallPlugin(new PluginFreeImage());
#endif
    }

    


    #pragma region ZoomScrollStateListener
    Vector2 OIV::GetImageSize()
    {
        return fOpenedImage.get() ? Vector2(static_cast<double>(fOpenedImage->GetWidth()), static_cast<double>(fOpenedImage->GetHeight()))
            : Vector2::ZERO;
    }

    Vector2 OIV::GetClientSize()
    {
        return Vector2(fClientWidth, fClientHeight);
    }
    
    void OIV::NotifyDirty()
    {
        Refresh();
    }
    #pragma endregion 

   

    void OIV::UpdateGpuParams()
    {
        Vector2 uvScaleFixed = fScrollState.GetARFixedUVScale();
        Vector2 uvOffset = fScrollState.GetOffset();
        if (fOpenedImage.get() == nullptr)
            uvScaleFixed = Vector2(1000000, 100000);
        
        fViewParams.showGrid = fShowGrid;
        fViewParams.uViewportSize = GetClientSize();
        fViewParams.uvOffset = uvOffset;
        fViewParams.uvscale = uvScaleFixed;
        
        fRenderer->SetViewParams(fViewParams);
    }

    void OIV::HandleWindowResize()
    {
        if (IsImageLoaded())
            fScrollState.Refresh();
    }

    bool OIV::IsImageLoaded() const
    {
        return fOpenedImage.get() != nullptr;
    }

    OIV_AxisAlignedRTransform OIV::ResolveExifRotation(unsigned short exifRotation) const
    {
        OIV_AxisAlignedRTransform rotation;
            switch (exifRotation)
            {
            case 3:
                rotation = AAT_Rotate180;
                break;
            case 6:
                rotation = AAT_Rotate90CW;
                break;
            case 8:
                rotation = AAT_Rotate90CCW;
                break;
            default:
                rotation = AAT_None;
            }
            return rotation;
    }

    IRendererSharedPtr OIV::CreateBestRenderer()
    {

#ifdef _MSC_VER 
     // Prefer Direct3D11 for windows.
    #if OIV_BUILD_RENDERER_D3D11 == 1
            return D3D11RendererFactory::Create();
    // Prefer Direct3D11 for windows.
    #elif  OIV_BUILD_RENDERER_GL == 1
        return GLRendererFactory::Create();
    #elif OIV_ALLOW_NULL_RENDERER == 1
        return IRendererSharedPtr(new NullRenderer());
    #else
        #error No valid renderers detected.
    #endif

#else // _MSC_VER
// If no windows choose GL renderer
    #if OIV_BUILD_RENDERER_GL == 1
        return GLRendererFactory::Create();
    #elif OIV_ALLOW_NULL_RENDERER == 1
        return IRendererSharedPtr(new NullRenderer());
    #else
        #error No valid renderers detected.
    #endif
#endif

        throw std::exception("wrong build configuration");

    }

#pragma region IPictureViewer implementation
    // IPictureViewr implementation
    int OIV::LoadFile(void* buffer, std::size_t size, char* extension, bool onlyRegisteredExtension)
    {
        ResultCode resultCode = RC_UknownError;
        
        ImageSharedPtr image = ImageSharedPtr(fImageLoader.LoadImage(buffer, size, extension, onlyRegisteredExtension));

        if (image.get())
        {
            fOpenedImage.swap(image);

            if (fOpenedImage.get() != nullptr)
            {

                using namespace easyexif;
                EXIFInfo exifInfo;

                if (exifInfo.parseFrom(static_cast<const unsigned char*>(buffer), static_cast<unsigned int>(size)) == PARSE_EXIF_SUCCESS)
                    fOpenedImage->Transform(ResolveExifRotation(exifInfo.Orientation));

                //TODO: send the renderer a copy of the converted image.
                fOpenedImage = ImageUtil::Convert(fOpenedImage, IT_BYTE_RGBA);

                if (fOpenedImage.get() != nullptr)
                {
                    if (fRenderer->SetImage(fOpenedImage) == RC_Success)
                    {
                        fScrollState.Reset(true);
                        resultCode = RC_Success;
                    }
                    else
                    {
                        resultCode = RC_PixelFormatConversionFailed;
                    }
                }
            }
            else
            {
                resultCode = RC_UnsupportedFormat;
            }
        }
        return resultCode;
    }

    double OIV::Zoom(double percentage, int x, int y)
    {
        fScrollState.Zoom(percentage, x, y);
        return 0.0;
    }

    int OIV::Pan(double x, double y)
    {
        fScrollState.Pan(Vector2(x, y));
        return 0.0;
    }

    int OIV::Init()
    {
        fRenderer = CreateBestRenderer();
        fRenderer->Init(fParent);
        return 0;
    }


    int OIV::SetParent(std::size_t handle)
    {
        fParent = handle;
        return 0;
    }
    int OIV::Refresh()
    {
        if (fIsRefresing == false)
        {
            fIsRefresing = true;
            HandleWindowResize();
            UpdateGpuParams();
            fRenderer->Redraw();
            fIsRefresing = false;
        }

        return 0;
    }

    Image* OIV::GetImage()
    {
        return fOpenedImage.get();
    }

    int OIV::SetFilterLevel(OIV_Filter_type filter_level)
    {
        if (filter_level >= FT_None && filter_level <= FT_Count)
        {
            fRenderer->SetFilterLevel(filter_level);
            Refresh();
            return RC_Success;
        }

        return RC_WrongParameters;
    }

    int OIV::GetFileInformation(QryFileInformation& information)
    {
        if (IsImageLoaded())
        {

            information.bitsPerPixel = fOpenedImage->GetBitsPerTexel();
            information.height = fOpenedImage->GetHeight();
            information.width = fOpenedImage->GetWidth();
            information.numMipMaps = 0;
            information.rowPitchInBytes = fOpenedImage->GetRowPitchInBytes();
            information.hasTransparency = 1;
            information.imageDataSize = 0;
            information.numChannels = 0;

            return RC_Success;
        }
        else
        {
            return 1;
        }
    }

    int OIV::GetTexelAtMousePos(int mouseX, int mouseY, double& texelX, double& texelY)
    {
        Vector2 texelPos = this->fScrollState.ClientPosToTexel(Vector2(mouseX, mouseY));
        texelX = texelPos.x;
        texelY = texelPos.y;
        return RC_Success;
    }

    int OIV::SetTexelGrid(double gridSize)
    {
        fShowGrid = gridSize > 0.0;
        Refresh();
        return RC_Success;
    }

    int OIV::GetNumTexelsInCanvas(double &x, double &y)
    {
        Vector2 canvasSize = fScrollState.GetNumTexelsInCanvas();
        x = canvasSize.x;
        y = canvasSize.y;
        return RC_Success;
    }

    int OIV::SetClientSize(uint16_t width, uint16_t height)
    {
        fClientWidth = width;
        fClientHeight = height;
        Refresh();
        return 0;
    }

    ResultCode OIV::AxisAlignTrasnform(const OIV_AxisAlignedRTransform transform)
    {
        if (fOpenedImage.get() != nullptr)
        {
            fOpenedImage->Transform(transform);
            if (fRenderer->SetImage(fOpenedImage) == RC_Success)
            {
                fScrollState.Reset(true);
                return RC_Success;
            }

        }
        return RC_UknownError;
    }
#pragma endregion

}
