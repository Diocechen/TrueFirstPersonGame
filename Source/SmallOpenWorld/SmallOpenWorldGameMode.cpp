// Copyright Epic Games, Inc. All Rights Reserved.

#include "SmallOpenWorldGameMode.h"
#include "SmallOpenWorldCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASmallOpenWorldGameMode::ASmallOpenWorldGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
