#pragma once
#include <API/defs.h>
#include <Image.h>


namespace OIV
{
    class IPictureRenderer
    {
    public:
        virtual ResultCode LoadFile(void* buffer, std::size_t size, char* extension, OIV_CMD_LoadFile_Flags flags, ImageHandle& handle) = 0;
        virtual ResultCode LoadRaw(const OIV_CMD_LoadRaw_Request& loadRawRequest, int16_t& handle) = 0;
        virtual ResultCode UnloadFile(const ImageHandle handle) = 0;
        //virtual ResultCode DisplayFile(const OIV_CMD_DisplayImage_Request& display_request) = 0;
        virtual ResultCode CreateText(const OIV_CMD_CreateText_Request&, OIV_CMD_CreateText_Response&) = 0;

        virtual ResultCode SetSelectionRect(const OIV_CMD_SetSelectionRect_Request& selectionRect) = 0;

        virtual int Init() = 0;
        virtual int SetParent(std::size_t handle) = 0;
        virtual int Refresh() = 0;

        virtual ResultCode GetFileInformation(ImageHandle handle, OIV_CMD_QueryImageInfo_Response& information) = 0;
        virtual IMCodec::ImageSharedPtr GetImage(ImageHandle handle) const = 0;
        virtual int SetTexelGrid(double gridSize) = 0;
        virtual int SetClientSize(uint16_t width, uint16_t height) = 0;
        virtual ResultCode AxisAlignTrasnform(const OIV_CMD_AxisAlignedTransform_Request& request, OIV_CMD_AxisAlignedTransform_Response& response) = 0;
        virtual ~IPictureRenderer() {}
        virtual ResultCode SetImageProperties(const OIV_CMD_ImageProperties_Request& imageProperties) = 0;
        
        virtual ResultCode CropImage(const OIV_CMD_CropImage_Request& oiv_cmd_get_pixel_buffer_request, OIV_CMD_CropImage_Response& oiv_cmd_get_pixel_buffer_response) = 0;
        virtual ResultCode GetPixels(const OIV_CMD_GetPixels_Request& req, OIV_CMD_GetPixels_Response& res) = 0;
        virtual ResultCode ConverFormat(const OIV_CMD_ConvertFormat_Request& req,OIV_CMD_ConvertFormat_Response& res) = 0;
        virtual ResultCode SetColorExposure(const OIV_CMD_ColorExposure_Request& exposure) = 0;
        virtual ResultCode GetTexelInfo(const OIV_CMD_TexelInfo_Request& texel_request, OIV_CMD_TexelInfo_Response& texelresponse) = 0;
        virtual ResultCode GetKnownFileTypes(OIV_CMD_GetKnownFileTypes_Response& res) = 0;
        virtual ResultCode GetSubImages(const OIV_CMD_GetSubImages_Request& request, OIV_CMD_GetSubImages_Response& res) = 0;

        virtual ResultCode RegisterCallbacks(const OIV_CMD_RegisterCallbacks_Request& callbacks) = 0;
    };
}
