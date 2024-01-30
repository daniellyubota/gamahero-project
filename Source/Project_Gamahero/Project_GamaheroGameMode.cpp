// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_GamaheroGameMode.h"
#include "Project_GamaheroCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProject_GamaheroGameMode::AProject_GamaheroGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/Character/GamaheroChar_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}


}
