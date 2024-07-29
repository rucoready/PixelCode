// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_2PhaseSwordRain.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UTask_2PhaseSwordRain : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
    UTask_2PhaseSwordRain(FObjectInitializer const& ObjectInitializer);

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
