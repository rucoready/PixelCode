// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_GruxAttack01.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UTask_GruxAttack01 : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	

public:

	explicit UTask_GruxAttack01(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	FVector nearlestPlayerLocation;

	bool animOnce;

	float currentTime;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class APixelCodeCharacter* player;

	class AGrux* grux;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector nearlestPlayerLocationKey;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float attackDelay = 4.0f;

	
	
};
