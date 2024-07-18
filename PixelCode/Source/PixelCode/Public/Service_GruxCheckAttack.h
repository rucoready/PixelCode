// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Service_GruxCheckAttack.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UService_GruxCheckAttack : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
    UService_GruxCheckAttack();


    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    FVector actorLoc;

    float stopDistance = 700.0f;

    bool canAttack = false;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector sensePlayer;

};
