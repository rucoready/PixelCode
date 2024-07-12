// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_BossIdleMove.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UTask_BossIdleMove : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
    UTask_BossIdleMove(FObjectInitializer const& ObjectInitializer);

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    float searchRadius = 150.0f;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    class APixelCodeCharacter* player;

    float currentTime;
};
