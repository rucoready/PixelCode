// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task_BossIdleMove.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h" //캐릭터 임시
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UTask_BossIdleMove::UTask_BossIdleMove(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Boss Idle Follow Move");


    bNotifyTick = true;
}

EBTNodeResult::Type UTask_BossIdleMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    TickTask(OwnerComp, NodeMemory, 0.0f);
    return EBTNodeResult::InProgress;
}

void UTask_BossIdleMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);



    ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (Player)
    {
        FVector PlayerLocation = Player->GetActorLocation();
        FNavLocation playerLocation;
        if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
        {
            if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, searchRadius, playerLocation))
            {
                OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), playerLocation.Location);
                ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
                if (bossController)
                {
                    bossController->MoveToLocation(PlayerLocation);

                    // 블랙보드 키로부터 값을 가져옴
                    bool value = OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
                    

                    // 조건 확인
                    if (value == true)
                    {
                        // 태스크 완료

                        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
                        //return;
                    }


                }


            }
        }

    }

}
