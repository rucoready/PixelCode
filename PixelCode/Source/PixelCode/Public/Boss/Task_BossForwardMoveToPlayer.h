// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_BossForwardMoveToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UTask_BossForwardMoveToPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
    UTask_BossForwardMoveToPlayer(FObjectInitializer const& ObjectInitializer);

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    float currentTime;
    float escapeTime;

    bool bFirstExecution = false;

    bool ComboAttack01Play = false;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    bool searchRandom = false;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    float searchRadius = 150.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
    class UAnimMontage* swordComboAttack1;

    UPROPERTY(EditAnywhere, Category = "Niagara")
    class UNiagaraSystem* impactSwing;

    UPROPERTY(EditAnywhere, Category = "Niagara")
    class UNiagaraSystem* impactSwing2;

    bool animOnce;

    bool swingSwordNiagaraOnce = false;
};
