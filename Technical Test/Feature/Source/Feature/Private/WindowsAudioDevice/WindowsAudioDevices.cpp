/****************** (C) Copyright 2019-2020 TW Rights Reserved. ******************
* Project Name:         RW1
* File Name:            WindowsAudioDevices.cpp
* Author:               Tao Wang
* Date First Issued:    01.10.2020
* Description:          Manage Windows audio device
**************************************************************************************************************/

#if PLATFORM_WINDOWS

#include "WindowsAudioDevice/WindowsAudioDevices.h"

#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "Mmdeviceapi.h"
#include "WindowsAudioDevice/PolicyConfig.h"
#include "Propidl.h"
#include "Functiondiscoverykeys_devpkey.h"

FString WindowsAudioDevices::CurrentDeviceId = FString();

void WindowsAudioDevices::ResetAudioDevice()
{
    ChangeAudioDevice(CurrentDeviceId);
}

void WindowsAudioDevices::ChangeAudioDevice(const FString deviceId)
{
    bool bSuccessChanged = false;
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (SUCCEEDED(hr))
    {
        IMMDeviceEnumerator* pEnum = NULL;
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
            (void**)&pEnum);
        if (SUCCEEDED(hr))
        {
            IMMDeviceCollection* pDevices;
            hr = pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDevices);
            if (SUCCEEDED(hr))
            {
                UINT count;
                pDevices->GetCount(&count);

                // Get default device
                IMMDevice* pDefaultDevice;
                hr = pEnum->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDefaultDevice);
                if (SUCCEEDED(hr))
                {
                    LPWSTR strDefaultDeviceID;
                    hr = pDefaultDevice->GetId(&strDefaultDeviceID);

                    CurrentDeviceId = FString(strDefaultDeviceID);
                    if (CurrentDeviceId == deviceId)
                    {
                        return;
                    }

                    // Iterate all devices
                    for (int32 i = 0; i <= (int32)count; ++i)
                    {
                        IMMDevice* pCurrentDevice;
                        hr = pDevices->Item(i, &pCurrentDevice);
                        if (SUCCEEDED(hr))
                        {
                            LPWSTR strID = NULL;
                            hr = pCurrentDevice->GetId(&strID);
                            if (FString(strID) == deviceId)
                            {
                                if (SUCCEEDED(hr))
                                {
                                    IPolicyConfigVista* pPolicyConfig;
                                    ERole reserved = eConsole;

                                    hr = CoCreateInstance(__uuidof(CPolicyConfigVistaClient),
                                        NULL, CLSCTX_ALL, __uuidof(IPolicyConfigVista), (LPVOID*)&pPolicyConfig);
                                    if (SUCCEEDED(hr))
                                    {
                                        hr = pPolicyConfig->SetDefaultEndpoint(strID, reserved);
                                        pPolicyConfig->Release();
                                        if (SUCCEEDED(hr))
                                        {
                                            bSuccessChanged = true;
                                        }
                                    }
                                }
                                break;
                            }
                            pCurrentDevice->Release();
                        }
                    }
                }
            }
            pEnum->Release();
        }
    }
    if (!bSuccessChanged)
    {
        return;
    }
}
#endif
