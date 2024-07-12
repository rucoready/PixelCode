// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_DodgetAddBack.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UTask_DodgetAddBack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
    UTask_DodgetAddBack(FObjectInitializer const& ObjectInitializer);

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    float currentTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
    class UAnimMontage* dodgeAndAttack01;

    UPROPERTY(EditAnywhere, Category = "Niagara")
    class UNiagaraSystem* doubleSwingNA2;

    UPROPERTY(EditAnywhere, Category = "Niagara")
    class UNiagaraSystem* doubleSwingNA3;

    bool animOnce;

    FVector playerLocation;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    class APixelCodeCharacter* player;

    bool onceSpawnStingNiagara = false;

    float speed = 500.0f;
    float distance = 200.0f;
};
