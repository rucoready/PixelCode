// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task_BossFirstEquitSword.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h" //캐릭터 임시
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"
#include "Boss/BossFirstPositionActor.h"
#include "Boss/BossApernia.h"

UTask_BossFirstEquitSword::UTask_BossFirstEquitSword(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Boss First Equit Sword Scene");
    currentTime = 0.0f;
    //escapeTime = 3.0f;
    bNotifyTick = true;
    
}

EBTNodeResult::Type UTask_BossFirstEquitSword::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    
    for (TActorIterator<ABossFirstPositionActor> It(GetWorld()); It; ++It)
    {
        ABossFirstPositionActor* PositionActor = *It;
        if (PositionActor)
        {
            FVector PositionActorLocation = PositionActor->GetActorLocation();
            OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PositionActorLocation);

            ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
            if (bossController)
            {
                bossController->MoveToLocation(PositionActorLocation);
            }
        }
    }

    if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
    {
        APawn* ControlledPawn = bossController->GetPawn();
        if (ControlledPawn)
        {
            ABossApernia* bossCharacter = Cast<ABossApernia>(ControlledPawn);

            // 애니메이션 몽타주를 재생합니다.
            if (swordEquitMT && bossCharacter->GetMesh() && bossCharacter->GetMesh()->GetAnimInstance() && !bSwordEquit)
            {
                bossCharacter->ServerRPC_SwordFirstEquip();
                bSwordEquit = true;

                OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bSwordEquit);
            }
        }
    }
    //TickTask(OwnerComp, NodeMemory, 0.0f);
    return EBTNodeResult::InProgress;
    
}

void UTask_BossFirstEquitSword::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (currentTime == 0.0f)
    {
        
    }
    currentTime += DeltaSeconds;

    // 3.5초가 지나면 태스크 완료
    if (currentTime >= 3.5f)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        
    }
}


