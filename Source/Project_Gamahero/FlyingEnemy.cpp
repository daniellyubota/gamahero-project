// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingEnemy.h"

#include "Project_GamaheroGameMode.h"
#include "SideScrollerCharacter.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"


AFlyingEnemy::AFlyingEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("MySceneComponent"));
    SceneComp->SetupAttachment(RootComponent);
    SceneComp->SetRelativeTransform(FTransform(FRotator(0.f, 0.f, 0.f), FVector(0.f, 0.f, 0.f), FVector(1.f, 1.f, 1.f)));
    MovementSpeed = 100.0f;
   
}

void AFlyingEnemy::BeginPlay()
{
    TargetLocation = SceneComp->GetComponentLocation();
	Super::BeginPlay();
    StartingLocation = GetActorLocation();
    USphereComponent* SphereCollision = FindComponentByClass<USphereComponent>();
    if (!SphereCollision)
    {

    }
    else
    {
        SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AFlyingEnemy::OnCollisionBegin);
    }
}

void AFlyingEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(TargetLocation != StartingLocation)
	{
    	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
    if (TargetLocation.Y > GetActorLocation().Y)
    {
        ActorRotation = GetActorRotation();
        ActorRotation.Yaw = 0;
        SetActorRotation(ActorRotation);
    }
    else if (TargetLocation.Y < GetActorLocation().Y)
    {
        ActorRotation = GetActorRotation();
        ActorRotation.Yaw = 180;
        SetActorRotation(ActorRotation);
    }
    FVector NewLocation = GetActorLocation() + (Direction * MovementSpeed * DeltaTime);

    if (FVector::DistSquared(NewLocation, TargetLocation) < 100.0f)
    {
        FVector Temp = TargetLocation;
        TargetLocation = StartingLocation;
        StartingLocation = Temp;
    }
    else
    {
        SetActorLocation(NewLocation);
    }
}
}
void AFlyingEnemy::OnCollisionBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ASideScrollerCharacter* PlayerCharacter = Cast<ASideScrollerCharacter>(OtherActor);
    if (PlayerCharacter && (PlayerCharacter->IsDashing))
    {
        DeathSound();
        Destroy();
        if (!PlayerCharacter->CanDash)
        {
            PlayerCharacter->DashAvailable();
        }
        PlayerCharacter->CanDash = true;
        PlayerCharacter->CanHoldLongDash = true;
        

    }
    else if (PlayerCharacter && (PlayerCharacter->IsLongDashing))
    {
        DeathSound();
        Destroy();
        if (!PlayerCharacter->CanDash)
        {
            PlayerCharacter->DashAvailable();
        }
        PlayerCharacter->CanDash = true;
        PlayerCharacter->CanHoldLongDash = true;
        


    }
    else if (PlayerCharacter && (PlayerCharacter->IsDashing == false && PlayerCharacter->IsLongDashing == false))
    {

        AProject_GamaheroGameMode* GameMode = (AProject_GamaheroGameMode*)GetWorld()->GetAuthGameMode();

        GameMode->Restart();


    }

}