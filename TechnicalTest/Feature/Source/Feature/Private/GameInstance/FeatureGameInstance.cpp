// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/FeatureGameInstance.h"

#if PLATFORM_WINDOWS
#include "Runtime/HeadMountedDisplay/Public/IHeadMountedDisplayModule.h"
#include "WindowsAudioDevice/WindowsAudioDevices.h"
#endif

UFeatureGameInstance::UFeatureGameInstance()
{
}

void UFeatureGameInstance::Init()
{
#if PLATFORM_WINDOWS
    if (IHeadMountedDisplayModule::IsAvailable())
    {
        const FString WindowsAudioDeviceName = IHeadMountedDisplayModule::Get().GetAudioOutputDevice();
        GConfig->SetString(
            TEXT("/Script/WindowsTargetPlatform.WindowsTargetSettings"),
            TEXT("AudioDevice"), *WindowsAudioDeviceName, GEngineIni);

        // Change the default audio device to VR audio device
        WindowsAudioDevices::ChangeAudioDevice(WindowsAudioDeviceName);
    }
#endif
}

void UFeatureGameInstance::Shutdown()
{
#if PLATFORM_WINDOWS
    // Reset the audio device to the user default setting
    WindowsAudioDevices::ResetAudioDevice();
#endif
}
