// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_BossMoveToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UTask_BossMoveToPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
    UTask_BossMoveToPlayer(FObjectInitializer const& ObjectInitializer);

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    float currentTime;
    float escapeTime;

    bool bFirstExecution = false;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    bool searchRandom = false;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    float searchRadius = 150.0f;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector canAttackBlackboardKey;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    class APixelCodeCharacter* player;

    
    

    


};
