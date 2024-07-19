// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Service_DogSense.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UService_DogSense : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
    UService_DogSense();


    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    bool canAttack = false;

    float currentTime;


    UPROPERTY(EditAnywhere, Category = "MySettings")
    class APixelCodeCharacter* player;	

    class ADogBart* dogBart;

    bool bAnyPlayerDetected = false;

    FVector actorLoc;

    float distance;

    FVector dogLoc;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector playerInSight;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector detectedPlayerName;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector detectedPlayerLoc;

    FString DetectedPlayerName;
};
