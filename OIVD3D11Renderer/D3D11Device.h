#pragma once
#include <windows.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include "D3D11Common.h"

namespace OIV
{
    class D3D11Device
    {
    public:

        D3D11Device()
        {
            
        }


        ~D3D11Device()
        {
            SAFE_RELEASE(fD3dSwapChain);

            //Destroy device
            SAFE_RELEASE(fD3dContext);
            SAFE_RELEASE(fD3dDevice);
        }


        ID3D11DeviceContext* GetContext()  const
        {
            return fD3dContext;
        }

        ID3D11Device* GetdDevice()  const
        {
            return fD3dDevice;
        }

        IDXGISwapChain* GetSwapChain()  const
        {
            return fD3dSwapChain;
        }

        void Create(HWND hwnd)
        {
            fHWND = hwnd;
            D3D_FEATURE_LEVEL requestedLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };
            D3D_FEATURE_LEVEL obtainedLevel;


            DXGI_SWAP_CHAIN_DESC scd = { 0 };

            scd.BufferCount = 1;
            scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
            scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
            scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

            scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
            scd.OutputWindow = fHWND;
            scd.SampleDesc.Count = 1;
            scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
            scd.Windowed = true;
            scd.BufferDesc.Width = 1280;
            scd.BufferDesc.Height = 800;
            scd.BufferDesc.RefreshRate.Numerator = 0;
            scd.BufferDesc.RefreshRate.Denominator = 1;

            UINT createFlags = 0;
            createFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;

#ifdef _DEBUG
            createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


            HRESULT res =
                D3D11CreateDeviceAndSwapChain(
                    nullptr,
                    D3D_DRIVER_TYPE_HARDWARE,
                    nullptr,
                    createFlags,
                    requestedLevels,
                    sizeof(requestedLevels) / sizeof(D3D_FEATURE_LEVEL),
                    D3D11_SDK_VERSION,
                    &scd,
                    &fD3dSwapChain,
                    &fD3dDevice,
                    &obtainedLevel,
                    &fD3dContext);
            
        }
    private:
        HWND fHWND = nullptr;
         ID3D11Device* fD3dDevice = nullptr;
         ID3D11DeviceContext* fD3dContext = nullptr;
         IDXGISwapChain* fD3dSwapChain = nullptr;

    };

}