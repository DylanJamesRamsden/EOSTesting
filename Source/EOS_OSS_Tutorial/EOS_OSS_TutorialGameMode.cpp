// Copyright Epic Games, Inc. All Rights Reserved.

#include "EOS_OSS_TutorialGameMode.h"

#include "EOSPlayerController.h"
#include "EOS_OSS_TutorialCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEOS_OSS_TutorialGameMode::AEOS_OSS_TutorialGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = AEOSPlayerController::StaticClass(); // Tutorial 2: Setting the PlayerController to our custom one.
}
