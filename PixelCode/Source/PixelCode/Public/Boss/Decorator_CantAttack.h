// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "Decorator_CantAttack.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UDecorator_CantAttack : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UDecorator_CantAttack();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
