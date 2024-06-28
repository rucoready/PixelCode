// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_KeepUpdateLocation.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UBTService_KeepUpdateLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_KeepUpdateLocation();

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector PlayerLocationKey;
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
	
};
