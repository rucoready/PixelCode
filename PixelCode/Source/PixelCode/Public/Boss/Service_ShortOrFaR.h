// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Service_ShortOrFaR.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UService_ShortOrFaR : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
    UService_ShortOrFaR();


    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    bool bShort = false;


};
