// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_DoubleSwing.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UTask_DoubleSwing : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

    UTask_DoubleSwing(FObjectInitializer const& ObjectInitializer);

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    class UAnimMontage* doubleSwingAttack;

    float currentTime;

    FVector playerLocation;

    float speedAddMovement = 3200.0f;

    bool firstSwingAttack = false;

    UPROPERTY(EditAnywhere, Category = "Niagara")
    class UNiagaraSystem* doubleSwingNA;

    UPROPERTY(EditAnywhere, Category = "Niagara")
    class UNiagaraSystem* doubleSwingNA2;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    TSubclassOf<UCameraShakeBase> cameraShakeOBJ;

    bool onceSpawnStingNiagara = false;
    bool onceSpawnStingNiagara2 = false;
    bool animOnce = false;
    bool animOnce2 = false;
    bool changeSwordPosition1 = false;
    float speed = 700.0f;
    float speed2 = 1400.0f;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector doubleSwingAttackCoolTime;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    class APixelCodeCharacter* player;

    bool doubleSwingAttack1;
};
