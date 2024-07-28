// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_Boss2PhaseGoUp.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BossFloor.h"
#include "Math/UnrealMathUtility.h"
#include "Boss/BossApernia.h"

UTask_Boss2PhaseGoUp::UTask_Boss2PhaseGoUp(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Dodge Right");

	bNotifyTick = true;
    
}

EBTNodeResult::Type UTask_Boss2PhaseGoUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    currentTime = 0.0f;
    onceParticleSpawn = false;
    TickTask(OwnerComp, NodeMemory, 0.0f);

    if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
    {
        APawn* controlledPawn = bossController->GetPawn();
        if (controlledPawn)
        {          
            boss = Cast<ABossApernia>(controlledPawn);
            if (boss)
            {
                UE_LOG(LogTemp, Warning, TEXT("Boss2PhaseOn"));
                
                
                boss->ServerRPC_Boss2phaseRoar();
                
            }
        }
    }
    
    return EBTNodeResult::InProgress;
    
}

void UTask_Boss2PhaseGoUp::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    currentTime += DeltaSeconds;
    if (currentTime > 3.0f&&!onceParticleSpawn)
    {
        boss->ServerRPC_RoarParticle();
        onceParticleSpawn = true;
    }
    if (currentTime > 4.5f)
    {
        boss->SetActorLocation(FVector(2470.000000, 4420.000000, 267.964356));
    }
    if (currentTime > 5.5f)
    {
        float LerpAlpha = FMath::Min(1.0f, (currentTime - 5.5f) / lerpDuration); 

        FVector CurrentLocation = boss->GetActorLocation();
        FVector TargetLocation = FVector(CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z + 1500.0f);

        FVector NewLocation = FMath::Lerp(CurrentLocation, TargetLocation, LerpAlpha);

        boss->SetActorLocation(NewLocation);
        if (!onceShield)
        {
            boss->phaseShieldComponent->SetVisibility(true);
            boss->bossSwordComp->SetVisibility(false);
        }
        
    }
    
    if (currentTime > 7.0f&&!onceSpawnGigantSword)
    {
        boss->ServerRPC_SpawnGigantSword();
        onceSpawnGigantSword = true;
        
    }
    if (currentTime > 9.5f&&!onceCameraShake)
    {
        onceCameraShake = true;
        boss->ServerRPC_GigantSwordCameraShake();
    }
}
