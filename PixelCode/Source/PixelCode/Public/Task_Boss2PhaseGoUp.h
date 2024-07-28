// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_Boss2PhaseGoUp.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UTask_Boss2PhaseGoUp : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
    UTask_Boss2PhaseGoUp(FObjectInitializer const& ObjectInitializer);

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    class TSubclassOf<class ABossFloor> bossFloor;

    float currentTime;

    class ABossApernia* boss;

    bool onceParticleSpawn = false;

    bool onceShield = false;

    bool onceSpawnGigantSword = false;

    bool onceCameraShake = false;

    float lerpDuration = 3.0f; 

    UPROPERTY(EditAnywhere, Category = "MySettings")
    class TSubclassOf<class ADemonSword> demonSwordFactory;


};
