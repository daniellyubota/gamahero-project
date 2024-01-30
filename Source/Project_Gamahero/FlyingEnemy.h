// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlyingEnemy.generated.h"

UCLASS()
class PROJECT_GAMAHERO_API AFlyingEnemy : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void OnCollisionBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	AFlyingEnemy();
	UPROPERTY(EditAnywhere)
		USceneComponent* SceneComp;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void DeathSound();
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
		float MovementSpeed;
	FVector StartingLocation;
	FVector TargetLocation;
	FRotator ActorRotation;

};

