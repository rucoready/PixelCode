// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "DogBart.h"
#include "Task_DogBartCheckBehind.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UTask_DogBartCheckBehind : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
    UTask_DogBartCheckBehind(FObjectInitializer const& ObjectInitializer);

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    float currentTime;

    bool animOnce;


    class ADogBart* dogBart;

    FVector dogLoc;

    FVector actorLoc;

    float distance;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    class APixelCodeCharacter* player;

    FVector NearestPlayerLocation;

    FRotator targetRotation;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector canAttack;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector detectedPlayer;

    bool CanAttack = false;
};
