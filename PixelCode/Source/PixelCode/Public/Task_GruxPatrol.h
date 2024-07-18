// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_GruxPatrol.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UTask_GruxPatrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
    UTask_GruxPatrol(FObjectInitializer const& ObjectInitializer);

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    class UNavigationSystemV1* navSys;

    float currentTime;
};
