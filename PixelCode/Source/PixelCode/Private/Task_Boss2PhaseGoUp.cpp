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
	NodeName = TEXT("2Phase Start Motion");

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
    if (currentTime > 4.5f &&!onceSetFirst2PhaseLocation)
    {
        boss->SetActorLocation(FVector(2470, 4420, 261));
        onceSetFirst2PhaseLocation = true;
    }
    if (currentTime > 5.5f)
    {
        if (!onceResetBossCurrentTime)
        {
            boss->currentTime = 0.0f;
            onceResetBossCurrentTime = true;
        }
        
        boss->ServerRPC_Boss2phaseGoUp();

        
        
    }
    
    if (currentTime > 7.0f&&!onceSpawnGigantSword)
    {
        boss->ServerRPC_SpawnGigantSword();
        onceSpawnGigantSword = true;
        
    }
    if (currentTime > 7.0f && !OnceSpawnDecalSword9)
    {
        boss->ServerRPC_SpawnDemonSword2Phase();
        OnceSpawnDecalSword9 = true;

    }

    if (currentTime > 7.0f && !OnceFireSpawn)
    {
        boss->ServerRPC_SpawnDecalSword6();
        OnceFireSpawn = true;

    }
    if (currentTime > 9.5f&&!onceCameraShake)
    {
        onceCameraShake = true;
        boss->ServerRPC_GigantSwordCameraShake();
    }
    if (currentTime > 10.0f && !OnceFireSpawn2)
    {
        OnceFireSpawn2 = true;
        boss->ServerRPC_SpawnDecalSword7();
    }

    if (currentTime > 10.0f&&!OnceSpawnDecalSword)
    {
        OnceSpawnDecalSword = true;
        boss->ServerRPC_SpawnDecalSword();
    }

    if (currentTime > 13.0f && !OnceSpawnDecalSword2)
    {
        OnceSpawnDecalSword2= true;
        boss->ServerRPC_SpawnDecalSword2();
    }

    if (currentTime > 13.2f && !OnceSpawnDecalSword3)
    {
        OnceSpawnDecalSword3 = true;
        boss->ServerRPC_SpawnDecalSword3();
    }

    if (currentTime > 15.0f && !OnceSpawnDecalSword4)
    {
        OnceSpawnDecalSword4 = true;
        boss->ServerRPC_SpawnDecalSword4();
    }

    if (currentTime > 17.0f && !OnceSpawnDecalSword5)
    {
        OnceSpawnDecalSword5 = true;
        boss->ServerRPC_SpawnDecalSword5();
    }
    if (currentTime > 17.0f && !OnceSpawnDecalSword6)
    {
        OnceSpawnDecalSword6 = true;
        boss->ServerRPC_SpawnDecalSword2();
    }
    if (currentTime > 17.5f && !OnceSpawnDecalSword7)
    {
        OnceSpawnDecalSword7 = true;
        boss->ServerRPC_SpawnDecalSword4();
    }

    if (currentTime > 19.0f && !OnceSpawnDecalSword8)
    {
        OnceSpawnDecalSword8 = true;
        boss->ServerRPC_SpawnDecalSword();
        boss->ServerRPC_SpawnDecalSword5();
    }
    if (currentTime > 22.0f && !OnceSpawnDecalSword10)
    {
        OnceSpawnDecalSword10 = true;
        boss->ServerRPC_SpawnDecalSword4();
        boss->ServerRPC_SpawnDecalSword5();
    }

    if (currentTime > 28.0f && !OnceSpawnDecalSword11)
    {
        OnceSpawnDecalSword11 = true;
        boss->ServerRPC_SpawnDecalSword();
        boss->ServerRPC_SpawnDecalSword2();
    }

    if (currentTime > 30.0f && !OnceSpawnDecalSword12)
    {
        OnceSpawnDecalSword12 = true;
        boss->ServerRPC_lastSpawnDecalSword1();
    }

    if (currentTime > 35.0f && !OnceSpawnDecalSword13)
    {
        OnceSpawnDecalSword13 = true;
        boss->ServerRPC_lastSpawnDecalSword2();
    }

    if (currentTime > 40.0f && !OnceSpawnDecalSword14)
    {
        OnceSpawnDecalSword14 = true;
        boss->ServerRPC_lastSpawnDecalSword1();
    }

    if (currentTime > 45.0f && !OnceSpawnDecalSword15)
    {
        OnceSpawnDecalSword15 = true;
        boss->ServerRPC_lastSpawnDecalSword2();
    }

    if (currentTime > 50.0f && !OnceSpawnDecalSword16)
    {
        OnceSpawnDecalSword16 = true;
        boss->ServerRPC_lastSpawnDecalSword1();
    }

    if (currentTime > 20.0f && !OnceSpawnStatue)
    {
        boss->ServerRPC_SpawnLazorDragonStatue();
        OnceSpawnStatue = true;
    }

//     if (currentTime > 60.0f)
//     {
//         boss->ServerRPC_CheckingStatueSurvive();
//     }
 
    if (boss)
    {
        if (boss->statueDestroyCount == 3)
        {

            OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
            boss->ServerRPC_DestroyShield();
            FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        }

        if (boss->statueDestroyCount != 3&&currentTime >70.0f)
        {
            boss->ServerRPC_SpawnAllDestroyActor();
        }
    }
    
}
