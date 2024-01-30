// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint_BP.h"
#include "Project_GamaheroGameMode.h"
#include "SideScrollerCharacter.h"
#include "Components/BoxComponent.h"
ACheckpoint_BP::ACheckpoint_BP()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACheckpoint_BP::BeginPlay()
{
	Super::BeginPlay();
	UBoxComponent* BoxCollision = Cast<UBoxComponent>(GetDefaultSubobjectByName(TEXT("Box")));
	if (!BoxCollision)
	{
	}
	else
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint_BP::OnOverlapBegin);
	}

}
void ACheckpoint_BP::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASideScrollerCharacter* PlayerCharacter = Cast<ASideScrollerCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		AProject_GamaheroGameMode* GameMode = Cast<AProject_GamaheroGameMode>(GetWorld()->GetAuthGameMode());

		GameMode->ActiveCheckpoint = this;
		GameMode->StartTimer(this);
		Activate();
	}
}

void ACheckpoint_BP::Activate()
{
	UBoxComponent* BoxCollision = Cast<UBoxComponent>(GetDefaultSubobjectByName(TEXT("Box")));
	BoxCollision->DestroyComponent();
}
void ACheckpoint_BP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

