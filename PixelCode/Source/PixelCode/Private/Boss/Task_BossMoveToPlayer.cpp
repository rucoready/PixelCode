// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task_BossMoveToPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h" //캐릭터 임시
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Boss/BossApernia.h"
#include "GameFramework/Character.h"



UTask_BossMoveToPlayer::UTask_BossMoveToPlayer(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Calculate Player Location");
    currentTime = 0.0f;
    escapeTime = 3.0f;
    bNotifyTick = true;
}

EBTNodeResult::Type UTask_BossMoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    currentTime = 0.0f;
    TickTask(OwnerComp, NodeMemory, 0.0f);

    return EBTNodeResult::InProgress;
    //return EBTNodeResult::Succeeded;


}

void UTask_BossMoveToPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


    currentTime += DeltaSeconds;

    if (currentTime >= 5.f)
    {

    }

    ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (Player)
    {
        FVector PlayerLocation = Player->GetActorLocation();


        FNavLocation RandomLocation;
        if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
        {

            if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, searchRadius, RandomLocation))
            {

                OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), RandomLocation.Location);


                // 블랙보드에서 키를 받아옴
                bool value = OwnerComp.GetBlackboardComponent()->GetValueAsBool(canAttackBlackboardKey.SelectedKeyName);
                //FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
                //FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
                // 조건 확인
                if (value == true)
                {
                    // 태스크 완료


                    FinishLatentTask(OwnerComp, EBTNodeResult::Failed);


                }

            }
        }

        else
        {
            OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
            return;
        }
    }

    return;

}





