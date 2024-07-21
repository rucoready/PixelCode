// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_AttackWait.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UTask_AttackWait : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	explicit UTask_AttackWait(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	FVector playerLocation;

	class ABossApernia* bossCharacter;

	float currentTime;

	bool taskOnceMove = false;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class APixelCodeCharacter* player;
};
