// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Service_DogBartCanAttackCheck.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UService_DogBartCanAttackCheck : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
    UService_DogBartCanAttackCheck();


    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    float currentTime;

    FVector NearestPlayerLocation;

	float CanAttackDistance = 400.0f;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector meleeAttackOn;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector jumpAttackOn;
    
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector moveOn;

    bool meleeAttack = false;

    bool jumpAttack = false;

    bool moveNow = false;

    class ADogBart* dogBart;

    FVector dogBartLoc;

    float distance;

    
};
