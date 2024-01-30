#include "SideScrollerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "dos.h"
#include "Project_GamaheroGameMode.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"


ASideScrollerCharacter::ASideScrollerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->JumpZVelocity = 1000.0f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;



}

void ASideScrollerCharacter::BeginPlay()
{
	Super::BeginPlay();


	Indicator = Cast<UMeshComponent>(GetDefaultSubobjectByName(TEXT("Plane")));
	Material = Indicator->GetMaterial(0);
	DynamicMaterial = Indicator->CreateDynamicMaterialInstance(0, Material);
	Linecast1 = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("Linecast1")));
	Linecast2 = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("Linecast2")));
	CanMove = true;
	CanDash = true;
	CanLongDash = false;
	CanHoldLongDash = true;
	IsDashing = false;
	IsLongDashing = false;
	DashCurrentCD = 0.0f;
	LongDashCurrentCD = 0.0f;
	HasReleasedDash = true;
	TimeCounter = 0;
	TimeCounter = 0;
	IsHoldingDash = false;
	IsMoving = false;
	IsSliding = false;
	StartCooldown = false;
	HasTouchedGround = true;
}

void ASideScrollerCharacter::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	TempPos = GetActorLocation();

	if (GetCharacterMovement()->MovementMode == MOVE_Walking && !IsDashing && !IsLongDashing && !IsHoldingDash && HasReleasedDash)
	{
		ASideScrollerCharacter::GetCharacterMovement()->GroundFriction = 3.0f;
		CanMove = true;
		IsDashing = false;
		IsLongDashing = false;
		HasTouchedGround = true;;
	}
	if (StartCooldown)
	{
		DashTimer -= DeltaTime;
		if (DashTimer <= 0 && HasTouchedGround)
		{
			if (!CanDash)
			{
				DashAvailable();
			}
			CanDash = true;
			CanHoldLongDash = true;
			CanLongDash = false;
			StartCooldown = false;

		}
	}
	if (IsDashing == true)
	{

		if (FVector::Distance(ActorLocation, TempPos) >= DashDistance || GetVelocity().Y == 0.0f)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Falling);
			GetCharacterMovement()->Velocity.Z = GetCharacterMovement()->Velocity.Z / 6;
			GetCharacterMovement()->Velocity.Y = GetCharacterMovement()->Velocity.Y / 6;

			ASideScrollerCharacter::GetCharacterMovement()->GroundFriction = 3.0f;

			CanMove = true;

			IsDashing = false;
			DashTimer = DashCooldown;
			StartCooldown = true;
			HasTouchedGround = false;
		}
	}
	if (IsLongDashing == true)
	{
		if (FVector::Distance(ActorLocation, TempPos) >= LongDashDistance || GetVelocity().Y == 0.0f)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Falling);
			GetCharacterMovement()->Velocity.Z = GetCharacterMovement()->Velocity.Z / 6;
			GetCharacterMovement()->Velocity.Y = GetCharacterMovement()->Velocity.Y / 6;

			ASideScrollerCharacter::GetCharacterMovement()->GroundFriction = 3.0f;

			CanMove = true;

			IsLongDashing = false;

			DashTimer = DashCooldown;
			StartCooldown = true;
			HasTouchedGround = false;
		}
	}
	if (!HasReleasedDash && CanHoldLongDash && IsHoldingDash)
	{
		const FVector ForwardDir = this->GetActorRotation().Vector();
		APlayerController* PlayerController = (APlayerController*)GetWorld()->GetFirstPlayerController();
		PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
		ActorLocation = this->GetActorLocation();

		MouseDirPlusLoc = FMath::LinePlaneIntersection(MouseLocation, MouseLocation + MouseDirection, FVector::Zero(), FVector::ForwardVector);
		FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(Indicator->GetComponentLocation(), MouseDirPlusLoc);
		Indicator->SetWorldRotation(PlayerRot);
		if (TimeDilation > 1 - HowMuchToSlow)
		{

			TimeCounter += DeltaTime;
			DynamicMaterial->SetScalarParameterValue("Opacity", TimeCounter / OverHowLong);
			TimeDilation = 1 - ((HowMuchToSlow * TimeCounter) / OverHowLong);
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
		}
		else if (TimeDilation <= 1 - HowMuchToSlow)
		{
			DynamicMaterial->SetVectorParameterValue("Color", FLinearColor::Red);
			TimeCounter2 += DeltaTime;
			CanLongDash = true;
		}
		if (TimeCounter2 >= HowMuchToWait)
		{
			DynamicMaterial->SetScalarParameterValue("Opacity", 0.0f);
			DynamicMaterial->SetVectorParameterValue("Color", FLinearColor::Yellow);
			HasReleasedDash = true;
			IsHoldingDash = false;
			CanLongDash = false;
			CanHoldLongDash = false;
			TimeCounter = 0;
			TimeCounter2 = 0;
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
			StartCooldown = true;
		}
	}

	FVector start1 = Linecast1->GetComponentLocation();
	FVector end1 = start1 + (GetActorForwardVector() * 20.0f);
	FVector start2 = Linecast2->GetComponentLocation();
	FVector end2 = start2 + (GetActorForwardVector() * 20.0f);

	FHitResult hit1;
	FHitResult hit2;
	bool actorHit1 = GetWorld()->LineTraceSingleByChannel(hit1, start1, end1, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());
	bool actorHit2 = GetWorld()->LineTraceSingleByChannel(hit2, start2, end2, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());


	if (actorHit1 && actorHit2 && hit1.GetActor()->IsRootComponentStatic() && hit2.GetActor()->
		IsRootComponentStatic() && GetCharacterMovement()->Velocity.Z < 0)
	{
		IsSliding = true;
		GetCharacterMovement()->Velocity.Z = -200;
	}
	else
	{
		IsSliding = false;
		GetCharacterMovement()->GravityScale = 3;
	}
	IsMoving = false;

}

void ASideScrollerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("PauseMenu", IE_Pressed, this, &ASideScrollerCharacter::PauseMenu).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("ResetLevel", IE_Pressed, this, &ASideScrollerCharacter::ResetGame);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASideScrollerCharacter::Dashing);
	PlayerInputComponent->BindAction("WallJump", IE_Pressed, this, &ASideScrollerCharacter::WallJump);
	PlayerInputComponent->BindAction("LongDash", IE_Released, this, &ASideScrollerCharacter::LongDashing);
	PlayerInputComponent->BindAction("LongDash", IE_Pressed, this, &ASideScrollerCharacter::ChargeLongDash);



	PlayerInputComponent->BindAxis("MoveRight", this, &ASideScrollerCharacter::MoveRight);



}
void ASideScrollerCharacter::ResetGame()
{

	AProject_GamaheroGameMode* GameMode = Cast<AProject_GamaheroGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->Restart();
}

void ASideScrollerCharacter::PauseMenu()
{
	AProject_GamaheroGameMode* GameMode = Cast<AProject_GamaheroGameMode>(GetWorld()->GetAuthGameMode());;
	GameMode->PauseMenu();
}



void ASideScrollerCharacter::MoveRight(float Value)
{

	if (CanMove)
	{
		if (IsSliding)
		{
			SlideI += 1;
		}
		if (SlideI == 0 || SlideI > 50 || !IsSliding)
		{
			SlideI = 0;
			AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Value);
			IsMoving = true;
		}
	}


}
void ASideScrollerCharacter::WallJump()
{
	if (IsSliding && GetCharacterMovement()->MovementMode != MOVE_Walking)
	{
		GetCharacterMovement()->StopMovementImmediately();
		FRotator ActorRotation = GetActorRotation();
		ActorRotation.Yaw = ActorRotation.Yaw + 180;
		SetActorRotation(ActorRotation);
		LaunchCharacter((GetActorForwardVector() + (GetActorUpVector() * 1.5)) * WallJumpForce, false, false);
		JumpSound();
	}
}
void ASideScrollerCharacter::Dashing()
{
	if (CanDash)
	{
		CanDash = false;
		CanHoldLongDash = false;
		CanLongDash = false;
		CanMove = false;
		const FVector ForwardDir = this->GetActorRotation().Vector();
		APlayerController* PlayerController = (APlayerController*)GetWorld()->GetFirstPlayerController();
		PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
		ActorLocation = this->GetActorLocation();

		MouseDirPlusLoc = FMath::LinePlaneIntersection(MouseLocation, MouseLocation + MouseDirection, FVector::Zero(), FVector::ForwardVector);
		NormVector = UKismetMathLibrary::GetDirectionUnitVector(ActorLocation, MouseDirPlusLoc);
		ASideScrollerCharacter::GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		LaunchCharacter(NormVector * DashForce, false, false);
		if (NormVector.Y < 0)
		{
			FRotator ActorRotation = GetActorRotation();
			ActorRotation.Yaw = -90;
			SetActorRotation(ActorRotation);
		}
		else
		{
			FRotator ActorRotation = GetActorRotation();
			ActorRotation.Yaw = 90;
			SetActorRotation(ActorRotation);
		}
		ASideScrollerCharacter::GetCharacterMovement()->GroundFriction = 0.5f;
		IsDashing = true;
		DashSound();

	}
}

void ASideScrollerCharacter::LongDashing()
{

	if (CanLongDash && IsHoldingDash && !HasReleasedDash)
	{
		CanMove = false;
		const FVector ForwardDir = this->GetActorRotation().Vector();
		APlayerController* PlayerController = (APlayerController*)GetWorld()->GetFirstPlayerController();
		PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
		ActorLocation = this->GetActorLocation();
		MouseDirPlusLoc = FMath::LinePlaneIntersection(MouseLocation, MouseLocation + MouseDirection, FVector::Zero(), FVector::ForwardVector);
		NormVector = UKismetMathLibrary::GetDirectionUnitVector(ActorLocation, MouseDirPlusLoc);
		ASideScrollerCharacter::GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		LaunchCharacter(NormVector * LongDashForce, false, false);
		if (NormVector.Y < 0)
		{
			FRotator ActorRotation = GetActorRotation();
			ActorRotation.Yaw = -90;
			SetActorRotation(ActorRotation);
		}
		else
		{
			FRotator ActorRotation = GetActorRotation();
			ActorRotation.Yaw = 90;
			SetActorRotation(ActorRotation);
		}
		ASideScrollerCharacter::GetCharacterMovement()->GroundFriction = 0.5f;
		IsLongDashing = true;

		TimeCounter = 0;
		TimeCounter2 = 0;
		CanLongDash = false;
		CanHoldLongDash = false;
		HasReleasedDash = true;
		IsHoldingDash = false;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
		CanDash = false;
		TimeDilation = 1;
		DynamicMaterial->SetScalarParameterValue("Opacity", 0.0f);
		DynamicMaterial->SetVectorParameterValue("Color", FLinearColor::Yellow);

		LongDashSound();
	}
	else if (!CanLongDash && IsHoldingDash && !HasReleasedDash)
	{
		TimeCounter = 0;
		TimeCounter2 = 0;
		CanLongDash = false;
		CanHoldLongDash = false;
		HasReleasedDash = true;
		IsHoldingDash = false;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
		CanDash = false;
		TimeDilation = 1;
		StartCooldown = true;
		DynamicMaterial->SetScalarParameterValue("Opacity", 0.0f);
		DynamicMaterial->SetVectorParameterValue("Color", FLinearColor::Yellow);

	}

}


void ASideScrollerCharacter::ChargeLongDash()
{
	if (CanHoldLongDash && HasReleasedDash && !IsHoldingDash)
	{
		CanDash = false;
		TimeCounter = 0;
		TimeCounter2 = 0;
		TimeDilation = 1;
		HasReleasedDash = false;
		IsHoldingDash = true;
	}
}


void ASideScrollerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

