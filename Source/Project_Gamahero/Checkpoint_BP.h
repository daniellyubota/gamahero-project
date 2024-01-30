// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint_BP.generated.h"

UCLASS()
class PROJECT_GAMAHERO_API ACheckpoint_BP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint_BP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
		void Activate();
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
