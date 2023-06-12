// Copyright Epic Games, Inc. All Rights Reserved.

#include "Basic/MyBohemianLifeGameMode.h"
#include "Character/MyBohemianLifeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMyBohemianLifeGameMode::AMyBohemianLifeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
