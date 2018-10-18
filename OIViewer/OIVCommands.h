#pragma once
#include "API/defs.h"
#include "API/functions.h"
#include <Rect.h>
#include <Exception.h>

namespace OIV
{
    class OIVCommands
    {
    public:

        static OIV_RECT_I ToOIVRect(const LLUtils::RectI32& rect)
        {
            LLUtils::RectI32::Point_Type topleft = rect.GetCorner(LLUtils::Corner::TopLeft);
            LLUtils::RectI32::Point_Type bottomRight = rect.GetCorner(LLUtils::Corner::BottomRight);
            return{ topleft.x, topleft.y, bottomRight.x, bottomRight.y};
        }

        template <class T, class U>
        static ResultCode ExecuteCommand(CommandExecute command, T* request, U* response)
        {
            return OIV_Execute(command, sizeof(T), request, sizeof(U), response);
        }

        static ResultCode Refresh()
        {
                return ExecuteCommand(CommandExecute::CE_Refresh, &CmdNull(), &CmdNull());
        }


        static ResultCode GetKnownFileTypes(std::string& o_fileTypes)
        {

            OIV_CMD_GetKnownFileTypes_Response res = {};

            ResultCode rc = RC_Success;

            if ((rc = ExecuteCommand(CommandExecute::OIV_CMD_GetKnownFileTypes, &CmdNull(), &res)) == RC_Success)
            {
                res.knownFileTypes = new char[res.bufferSize];
                if ((rc = ExecuteCommand(CommandExecute::OIV_CMD_GetKnownFileTypes, &CmdNull(), &res)) == RC_Success)
                    o_fileTypes = res.knownFileTypes;

                delete[] res.knownFileTypes;
            }
            return rc;
            
        }

        static ResultCode UnloadImage(ImageHandle handle)
        {
            if (handle != ImageHandleNull)
            {
                OIV_CMD_UnloadFile_Request unloadRequest = {};
                unloadRequest.handle = handle;
                return ExecuteCommand(CommandExecute::OIV_CMD_UnloadFile, &unloadRequest, &CmdNull());
            }
            else return RC_InvalidHandle;
        }

        static ResultCode TransformImage(ImageHandle handle, OIV_AxisAlignedRotation rotation, OIV_AxisAlignedFlip flip, ImageHandle& tranformedHandle)
        {
            OIV_CMD_AxisAlignedTransform_Request request = {};
            OIV_CMD_AxisAlignedTransform_Response response= {};
            
            
            request.handle = handle;
            request.transform.rotation = rotation;
            request.transform.flip= flip;
            ResultCode result = ExecuteCommand(CommandExecute::OIV_CMD_AxisAlignedTransform, &request, &response);
            if (result == RC_Success)
            {
                tranformedHandle = response.handle;
            }

            return result;
        }

        static ResultCode ConvertImage(ImageHandle handle, OIV_TexelFormat desiredTexelFormat,bool useRainbow, ImageHandle& converted)
        {
            OIV_CMD_ConvertFormat_Request request = {};
            OIV_CMD_ConvertFormat_Response response = {};
            request.handle = handle;
            request.format = desiredTexelFormat;
            request.flags = useRainbow ? OIV_CF_RAINBOW_NORMALIZE : static_cast<OIV_ConvertFormat_Flags>(0);
            ResultCode result = ExecuteCommand(CommandExecute::OIV_CMD_ConvertFormat, &request, &response);
            if (result == RC_Success)
                converted = response.handle;


            return result;
        }
        

        static ResultCode OIVCommands::CropImage(ImageHandle sourceImage, const LLUtils::RectI32& rect, ImageHandle& croppedHandle)
        {
            OIV_CMD_CropImage_Request requestCropImage;
            OIV_CMD_CropImage_Response responseCropImage;

            requestCropImage.rect = ToOIVRect(rect);
            requestCropImage.imageHandle = sourceImage;

            // 1. create a new cropped image 
            ResultCode result = ExecuteCommand(OIV_CMD_CropImage, &requestCropImage, &responseCropImage);
            croppedHandle = (result == RC_Success ? responseCropImage.imageHandle : ImageHandleNull);
            return result;
        }

        static void SetSelectionRect(const LLUtils::RectI32& rect)
        {
            OIV_CMD_SetSelectionRect_Request request;
            request.rect.x0 = rect.GetCorner(LLUtils::TopLeft).x;
            request.rect.y0 = rect.GetCorner(LLUtils::TopLeft).y;
            request.rect.x1 = rect.GetCorner(LLUtils::BottomRight).x;
            request.rect.y1 = rect.GetCorner(LLUtils::BottomRight).y;
            ExecuteCommand(CommandExecute::OIV_CMD_SetSelectionRect, &request, &CmdNull());
        }

        static void CancelSelectionRect()
        {
            OIV_CMD_SetSelectionRect_Request request = { -1,-1,-1,-1 };
            ExecuteCommand(CommandExecute::OIV_CMD_SetSelectionRect, &request, &CmdNull());
        }
        static void Init(HANDLE hwnd)
        {
            // Init OIV renderer
            CmdDataInit init;
            init.parentHandle = reinterpret_cast<std::size_t>(hwnd);
            if (ExecuteCommand(CommandExecute::CE_Init, &init, &CmdNull()) != RC_Success)
                LL_EXCEPTION(LLUtils::Exception::ErrorCode::RuntimeError, "Unable initialize OIV library");
            
        }
    };
}
