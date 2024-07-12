// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_Dodge02.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UTask_Dodge02 : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
    UTask_Dodge02(FObjectInitializer const& ObjectInitializer);

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    float currentTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
    class UAnimMontage* dodge02;

    bool animOnce;

    FVector playerLocation;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector dodgeLeftCoolTime;

    bool dodgeLeft;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    class APixelCodeCharacter* player;
};
