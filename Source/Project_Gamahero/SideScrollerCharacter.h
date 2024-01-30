// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SideScrollerCharacter.generated.h"

UCLASS()
class PROJECT_GAMAHERO_API ASideScrollerCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ASideScrollerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

	//UPROPERTY(VisibleAnywhere)
		//class UCameraComponent* SideViewCamera;

protected:

	void MoveRight(float Value);
	void Dashing();
	void WallJump();
	void LongDashing();
	void ChargeLongDash();
	void ResetGame();
	void PauseMenu();

public:

	void RestartLevel();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void JumpSound();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void LongDashSound();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void DashSound();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void DashAvailable();

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	bool CanMove, CanDash, CanLongDash, CanHoldLongDash, HasReleasedDash, IsMoving, StartCooldown, HasTouchedGround;
	float DashCurrentCD;
	float LongDashCurrentCD;
	float SlideI;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool IsDashing = false;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool IsLongDashing = false;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool IsHoldingDash = false;
private:
	UPROPERTY(EditAnywhere)
	float DashForce = 2500.0f;
	UPROPERTY(EditAnywhere)
		float WallJumpForce = 1000.0f;
	UPROPERTY(EditAnywhere)
	float DashDistance = 500.0f;
	UPROPERTY(EditAnywhere)
	float DashCooldown = 2.0f;
		float DashTimer;
	UPROPERTY(EditAnywhere)
		float LongDashForce = 2000.0f;
	UPROPERTY(EditAnywhere)
		float LongDashDistance = 1000.0f;
	UPROPERTY(EditAnywhere)
	float HowMuchToSlow = 0.6f;
	UPROPERTY(EditAnywhere)
	float OverHowLong = 2.0f;
	UPROPERTY(EditAnywhere)
		float HowMuchToWait = 2.0f;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool IsSliding = false;
	

	float TimeDilation = 0.0f;
	float TimeCounter = 0.0f;
	float TimeCounter2 = 0.0f;
	FVector TempPos, MouseLocation, MouseDirection, ActorLocation, NormVector, MouseDirPlusLoc;
	USceneComponent* Linecast1;
	USceneComponent* Linecast2;
	UMeshComponent* Indicator;
	UMaterialInterface* Material;
	UMaterialInstanceDynamic* DynamicMaterial;
	
};
