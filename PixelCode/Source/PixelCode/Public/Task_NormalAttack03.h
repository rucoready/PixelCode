// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_NormalAttack03.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UTask_NormalAttack03 : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
    UTask_NormalAttack03(FObjectInitializer const& ObjectInitializer);

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    float currentTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
    class UAnimMontage* swordNormalAttack04;

    FVector playerLocation;

    FVector moveDirection;

    bool directionSet = false;

    float speed = 100.0f;
    float speed2 = 150.0f;
    float speed3 = 300.0f;
    float speed4 = 720.0f;
    float speed5 = 1200.0f;
    float superSpeed = 2200.0f;

    bool animOnce;

    bool normalAttack03;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    class APixelCodeCharacter* player;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector normalAttack03CoolTime;
};
