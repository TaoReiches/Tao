// Copyright Epic Games, Inc. All Rights Reserved.

#include "TaoGameMode.h"
#include "TaoPlayerController.h"
#include "TaoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATaoGameMode::ATaoGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATaoPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}