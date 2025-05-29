// Copyright Epic Games, Inc. All Rights Reserved.

#include "Merc_GameGameMode.h"
#include "Merc_GameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMerc_GameGameMode::AMerc_GameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
