/****************** (C) Copyright 2019-2020 TW Rights Reserved. ******************
* Project Name:         TW
* File Name:            WindowsAudioDevices.h
* Author:               Tao Wang
* Date First Issued:    01.10.2020
* Description:          Manage Windows audio device
**************************************************************************************************************/

#pragma once

#if PLATFORM_WINDOWS

#include "CoreMinimal.h"

/**
 * Change the default audio device to the HMD device when the app starts
 * and reset it to the user default audio device when the app exit.
 */
class FEATURE_API WindowsAudioDevices
{
public:
    static void ChangeAudioDevice(const FString deviceId);
    static void ResetAudioDevice();

private:
    static FString CurrentDeviceId;
};

#endif