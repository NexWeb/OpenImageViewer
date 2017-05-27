#pragma once

#include <IImagePlugin.h>

#include <tiffio.h>
#include <tiffio.hxx>
#include "TiffClientFunctions.h"
#include "TiffFile.h"

namespace IMCodec
{
    class CodecTiff : public IImagePlugin
    {
    private:
        PluginProperties mPluginProperties;
    public:

        CodecTiff() : mPluginProperties({ "LibTiff image codec","tif;tiff" })
        {

        }

        PluginProperties& GetPluginProperties() override
        {
            return mPluginProperties;
        }

        //Base abstract methods
        bool LoadImage(const uint8_t* buffer, std::size_t size, ImageProperies& out_properties) override
        {
            using namespace std;

            bool success = false;
            TiffFile tifFile(buffer, size);
            TIFF* tiff = tifFile.GetTiff();

            if (tiff != nullptr)
            {
                uint16_t sampleFormat = 0;

                uint32 width, height;
                uint16 bitsPerSample;
                uint16 compression;
                uint16 photoMetric;
                uint16 samplesPerPixel;

                TexelFormat texelFormat = TexelFormat::TF_UNKNOWN;
                uint32_t rowPitch;
                uint8_t* decompressedBuffer = nullptr;
                uint32 rowsPerStrip;
                uint16_t stripRowCount = 0;


                TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bitsPerSample);
                TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &width);
                TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &height);
                TIFFGetField(tiff, TIFFTAG_COMPRESSION, &compression);
                TIFFGetField(tiff, TIFFTAG_PHOTOMETRIC, &photoMetric);
                TIFFGetField(tiff, TIFFTAG_SAMPLESPERPIXEL, &samplesPerPixel);
                TIFFGetField(tiff, TIFFTAG_SAMPLEFORMAT, &sampleFormat);
                TIFFGetField(tiff, TIFFTAG_ROWSPERSTRIP, &rowsPerStrip);
                TIFFGetField(tiff, TIFFTAG_STRIPROWCOUNTS, &stripRowCount);

                uint32_t numberOfStripts = TIFFNumberOfStrips(tiff);
                tmsize_t stripSize = TIFFStripSize(tiff);
                rowPitch = TIFFScanlineSize(tiff);

                switch (photoMetric)
                {
                case PHOTOMETRIC_RGB:
                    texelFormat = TF_I_R8_G8_B8_A8;
                    //override target row pitch - always 32bpp
                    rowPitch = width * 4;
                    decompressedBuffer = new uint8[height * rowPitch];
                    
                    TIFFReadRGBAImage(tiff, width, height, reinterpret_cast<uint32*>(decompressedBuffer));
                    break;
                case PHOTOMETRIC_MINISWHITE:
                case PHOTOMETRIC_MINISBLACK:
                {
                    switch (sampleFormat)
                    {
                    case SAMPLEFORMAT_IEEEFP:
                        switch (bitsPerSample)
                        {
                        case 16:
                            texelFormat = TF_F_X16;
                            break;
                        case 24:
                            texelFormat = TF_F_X24;
                            throw std::logic_error("not implemented");
                            break;
                        case 32:
                            texelFormat = TF_F_X32;
                            break;
                        default:
                            throw std::logic_error("unsupported format");
                        }

                        decompressedBuffer = new uint8[height * rowPitch];
                        uint8_t* currensPos = decompressedBuffer;
                        
                        if (stripSize != rowPitch * rowsPerStrip)
                            throw std::logic_error("Not implemented");

                        for (int i = 0; i < numberOfStripts; i++)
                        {
                            TIFFReadRawStrip(tiff, i, currensPos, stripSize);
                            currensPos += rowPitch * rowsPerStrip;
                        }

                        break;
                    }

                }
                }

                success = true;
                out_properties.Width = width;
                out_properties.Height = height;
                out_properties.NumSubImages = 0;
                out_properties.ImageBuffer = decompressedBuffer;
                out_properties.TexelFormatDecompressed = texelFormat;
                out_properties.RowPitchInBytes = rowPitch;
            }
            return success;
        }
    };
}
