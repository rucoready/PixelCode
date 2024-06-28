// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_FindPlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UBTService_FindPlayerLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_FindPlayerLocation();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search Settings")
	bool bSearchRandom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search Settings")
	float SearchRadius;
};
