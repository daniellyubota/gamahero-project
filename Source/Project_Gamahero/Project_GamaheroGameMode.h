// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Project_GamaheroGameMode.generated.h"

UCLASS(minimalapi)
class AProject_GamaheroGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProject_GamaheroGameMode();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAcess = "true"))
		TArray<AActor*> CheckpointsArray;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void StartTimer(AActor* checkpoint);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void Restart();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OpenTheLevel();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void PauseMenu();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAcess = "true"))
		AActor* ActiveCheckpoint;

};



