// Copyright Epic Games, Inc. All Rights Reserved.

#include "FeatureGameMode.h"
#include "FeaturePlayerController.h"
#include "FeatureCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFeatureGameMode::AFeatureGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AFeaturePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}