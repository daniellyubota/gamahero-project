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
	// Sets default values for this actor's properties
	AFlyingEnemy();
	UPROPERTY( EditAnywhere)
	USceneComponent* SceneComp;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void DeathSound();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	float MovementSpeed;
	FVector StartingLocation;
	FVector TargetLocation;
	FRotator ActorRotation;

};

