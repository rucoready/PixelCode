// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Service_Grux_SensingCheck.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UService_Grux_SensingCheck : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
    UService_Grux_SensingCheck();


    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    bool canAttack = false;

    float currentTime;


    UPROPERTY(EditAnywhere, Category = "MySettings")
    class APixelCodeCharacter* player;

    float senseRadius = 1500.0f;

    bool bPlayerInSight = false;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector canAttackKey;

    
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector nearlestPlayerLocation;

    bool bGuxCanAttack = false;

    float betweenSize;

    FVector actorLoc;

    float distance;

    FVector gruxLoc;

    class AGrux* grux;

    bool bOnceTimer = false;

    bool bAnyPlayerDetected = false;

    void CheckBehindAfterAttack(UBlackboardComponent* BlackboardComp);

    FTimerHandle timerhandle_CheckBegindAfterAttack;

    bool drawOnceLinetrace360 = false;

    float attackWaitTime;

    bool bCheckingBehind = false;

    float checkBehindStartTime;

    FVector targetDirection;

    bool attack01Once = false;
};
