// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_CounterPrecursor.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UTask_CounterPrecursor : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
    UTask_CounterPrecursor(FObjectInitializer const& ObjectInitializer);

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
    class UAnimMontage* counterPrecursor;

    bool animOnce;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    class APixelCodeCharacter* player;

    FVector playerLocation;

    float currentTime;
};
