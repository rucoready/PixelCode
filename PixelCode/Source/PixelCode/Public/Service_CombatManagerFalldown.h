// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Service_CombatManagerFalldown.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UService_CombatManagerFalldown : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UService_CombatManagerFalldown();


	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	float currentTime;

	bool bossFallDownOverlap = false;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector bFallDown;
};
