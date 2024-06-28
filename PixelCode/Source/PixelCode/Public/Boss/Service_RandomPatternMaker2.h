// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Service_RandomPatternMaker2.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UService_RandomPatternMaker2 : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
    UService_RandomPatternMaker2();


    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    bool canAttack = false;
};
