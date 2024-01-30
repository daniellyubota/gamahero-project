#include "GroundEnemy.h"

#include "Project_GamaheroGameMode.h"
#include "SideScrollerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AGroundEnemy::AGroundEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
    SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("MySceneComponent"));
    SceneComp->SetupAttachment(RootComponent);
    SceneComp->SetRelativeTransform(FTransform(FRotator(0.f, 0.f, 0.f), FVector(0.f, 0.f, 0.f), FVector(1.f, 1.f, 1.f)));

}

void AGroundEnemy::BeginPlay()
{
    Super::BeginPlay();
    Linecast = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("Linecast")));
    SecondLocation = SceneComp->GetComponentLocation();
    TargetLocation = SceneComp->GetComponentLocation();

    InitialLocation = GetActorLocation();
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    UCapsuleComponent* CapsuleCollision = Cast<UCapsuleComponent>(GetDefaultSubobjectByName(TEXT("Capsule")));
    UBoxComponent* BoxCollision = Cast<UBoxComponent>(GetDefaultSubobjectByName(TEXT("RangeBox")));
    USphereComponent* SphereCollision = Cast<USphereComponent>(GetDefaultSubobjectByName(TEXT("RangeSphere")));

    if (!CapsuleCollision || !BoxCollision || !SphereCollision)
    {
    }
    else
    {
        CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &AGroundEnemy::OnCollisionBeginEnemy);
        BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AGroundEnemy::OnCollisionBeginBox);
        BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AGroundEnemy::OnCollisionEndBox);
        SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AGroundEnemy::OnCollisionBeginSphere);
        SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AGroundEnemy::OnCollisionEndSphere);
    }

}
void AGroundEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (IsTouchingBox && IsTouchingSphere)
    {
        FVector start1 = GetActorLocation();
        FVector end1 = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
    	actorHit1 = GetWorld()->LineTraceSingleByChannel(hit1, start1, end1, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());


        FVector start2 = Linecast->GetComponentLocation();
        FVector end2 = start2 + (GetActorUpVector() * -100.0f);;
        actorHit2 = GetWorld()->LineTraceSingleByChannel(hit2, start2, end2, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());

    }
    
        if(IsTouchingSphere && IsTouchingBox && actorHit1 && hit1.GetActor() == GetWorld()->GetFirstPlayerController()->GetPawn())
        {
            if (GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().Y + 50.0f < GetActorLocation().Y)
            {
                ActorRotation = GetActorRotation();
                ActorRotation.Yaw = 180.0f;
                SetActorRotation(ActorRotation);
                if (actorHit2 && hit2.GetActor()->IsRootComponentStatic())
                {
                    SetActorLocation(GetActorLocation() - FVector(0.0f, MovementSpeed * DeltaTime, 0.0f));
                }
            }
            else if (GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().Y - 50.0f > GetActorLocation().Y)
            {
                ActorRotation = GetActorRotation();
                ActorRotation.Yaw = 0.0f;
                SetActorRotation(ActorRotation);
                
                if (actorHit2 && hit2.GetActor()->IsRootComponentStatic())
                {
                    SetActorLocation(GetActorLocation() + FVector(0.0f, MovementSpeed * DeltaTime, 0.0f));
                }
            }
        }
        else
        {
            
                if (GetActorLocation().Y != TargetLocation.Y && InitialLocation != SecondLocation)
                {
                    if (GetActorLocation().Y >= TargetLocation.Y)
                    {
                        ActorRotation = GetActorRotation();
                        ActorRotation.Yaw = 180.0f;
                        SetActorRotation(ActorRotation);
                        bIsMovingRight = false;
                        SetActorLocation(GetActorLocation() - FVector(0.0f, MovementSpeed * DeltaTime, 0.0f));
                        if (GetActorLocation().Y < TargetLocation.Y)
                        {
                            if (TargetLocation == InitialLocation)
                            {
                                TargetLocation = SecondLocation;
                            }
                            else if (TargetLocation == SecondLocation)
                            {
                                TargetLocation = InitialLocation;
                            }
                        }
                    }
                    else if (GetActorLocation().Y <= TargetLocation.Y)
                    {
                        ActorRotation = GetActorRotation();
                        ActorRotation.Yaw = 0.0f;
                        SetActorRotation(ActorRotation);
                        bIsMovingRight = true;
                        SetActorLocation(GetActorLocation() + FVector(0.0f, MovementSpeed * DeltaTime, 0.0f));
                        if (GetActorLocation().Y > TargetLocation.Y)
                        {
                            if (TargetLocation == InitialLocation)
                            {
                                TargetLocation = SecondLocation;
                            }
                            else if (TargetLocation == SecondLocation)
                            {
                                TargetLocation = InitialLocation;
                            }
                        }
                    }
                }
            
        }
    }


void AGroundEnemy::OnCollisionBeginEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
void AGroundEnemy::OnCollisionBeginSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ASideScrollerCharacter* PlayerCharacter = Cast<ASideScrollerCharacter>(OtherActor);
    if (PlayerCharacter)
    {
        IsTouchingSphere = true;


    }
}

void AGroundEnemy::OnCollisionBeginBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ASideScrollerCharacter* PlayerCharacter = Cast<ASideScrollerCharacter>(OtherActor);
    if (PlayerCharacter)
    {
        IsTouchingBox = true;


    }
}
void AGroundEnemy::OnCollisionEndSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ASideScrollerCharacter* PlayerCharacter = Cast<ASideScrollerCharacter>(OtherActor);
    if (PlayerCharacter)
    {
        IsTouchingSphere = false;
    }
}

void AGroundEnemy::OnCollisionEndBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ASideScrollerCharacter* PlayerCharacter = Cast<ASideScrollerCharacter>(OtherActor);
    if (PlayerCharacter)
    {
        IsTouchingBox = false;
    }
}