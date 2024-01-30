// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GroundEnemy.generated.h"

UCLASS()
class PROJECT_GAMAHERO_API AGroundEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGroundEnemy();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		UCapsuleComponent* RootCapsuleComponent;
	UPROPERTY(EditAnywhere)
		USceneComponent* SceneComp;
protected:
	UFUNCTION()
		void OnCollisionBeginEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnCollisionBeginSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnCollisionEndSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnCollisionBeginBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnCollisionEndBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void DeathSound();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector InitialLocation;
	FVector SecondLocation;
	FVector TargetLocation;
	FTimerHandle MoveTimerHandle;
	bool bIsMovingRight;
	UPROPERTY(EditAnywhere)
	float MovementSpeed = 200.0f;
	FRotator ActorRotation;
	bool IsTouchingBox, IsTouchingSphere, actorHit1, actorHit2;
	FHitResult hit1, hit2;

	USceneComponent* Linecast;
};
