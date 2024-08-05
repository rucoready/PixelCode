// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_DogBartMoveToPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DogBartAIController.h"
#include "DogBart.h"
#include "NavigationSystem.h"


UTask_DogBartMoveToPlayer::UTask_DogBartMoveToPlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Move To nearest Player");
}

EBTNodeResult::Type UTask_DogBartMoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if (BlackboardComp)
    {

        // 블랙보드에서 벡터 값을 가져옵니다.
        FVector NearestPlayerLocation = BlackboardComp->GetValueAsVector(TEXT("DetectedPlayerLoc"));

        if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
        {
            ADogBartAIController* dogBartController = Cast<ADogBartAIController>(OwnerComp.GetAIOwner());
            if (dogBartController)
            {
                
                dogBartController->MoveToLocation(NearestPlayerLocation);
            }
            if (dogBartController == NULL)
            {
                
            }
        }

        
    }

    return EBTNodeResult::Failed;
}

void UTask_DogBartMoveToPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    
}
