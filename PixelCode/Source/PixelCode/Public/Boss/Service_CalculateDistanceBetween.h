// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Service_CalculateDistanceBetween.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UService_CalculateDistanceBetween : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
    UService_CalculateDistanceBetween();


    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    bool canAttack = false;

    float currentTime;

    float timeToSelectPlayer;


    UPROPERTY(EditAnywhere, Category = "MySettings")
    class APixelCodeCharacter* player;
};
