// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task_CalculateFirstPositionActor.h"
#include "Boss/BossFirstPositionActor.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EngineUtils.h"

UTask_CalculateFirstPositionActor::UTask_CalculateFirstPositionActor(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Calculate First P Actor");
    
}

EBTNodeResult::Type UTask_CalculateFirstPositionActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    for (TActorIterator<ABossFirstPositionActor> It(GetWorld()); It; ++It)
    {
        ABossFirstPositionActor* PositionActor = *It;
        if (PositionActor)
        {
            FVector PositionActorLocation = PositionActor->GetActorLocation();
            OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PositionActorLocation);




        }
    }

    return EBTNodeResult::Succeeded;
    
}

    



