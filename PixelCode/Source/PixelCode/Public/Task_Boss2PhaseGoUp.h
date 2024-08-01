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

    bool OnceSpawnDecalSword = false;

    bool OnceSpawnDecalSword2 = false;

    bool OnceSpawnDecalSword3 = false;

    bool OnceSpawnDecalSword4 = false;

    bool OnceSpawnDecalSword5 = false;

    bool OnceSpawnDecalSword6 = false;

    bool OnceSpawnDecalSword7 = false;

    bool OnceSpawnDecalSword8 = false;

    bool OnceSpawnDecalSword9 = false;

    bool OnceSpawnDecalSword10 = false;

    bool OnceSpawnDecalSword11 = false;

    bool OnceSpawnDecalSword12 = false;

    bool OnceSpawnDecalSword13 = false;

    bool OnceSpawnDecalSword14 = false;

    bool OnceSpawnDecalSword15 = false;

    bool OnceSpawnDecalSword16 = false;

    bool OnceFireSpawn = false;

    bool OnceFireSpawn2 = false;

    bool OnceSpawnStatue = false;

    float lerpDuration = 3.0f; 

    bool OnceMove = false;

    bool onceResetBossCurrentTime = false;

    bool onceSetFirst2PhaseLocation = false;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    class TSubclassOf<class ADemonSword> demonSwordFactory;

    


};
