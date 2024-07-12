// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_JumpAttack01.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UTask_JumpAttack01 : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
    UTask_JumpAttack01(FObjectInitializer const& ObjectInitializer);

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    

    UPROPERTY(EditAnywhere, Category = "Niagara")
    class UNiagaraSystem* groundImpact02Niagara;

    UPROPERTY(EditAnywhere, Category = "Niagara")
    class UNiagaraSystem* groundImpact03Niagara;

    UPROPERTY(EditAnywhere, Category = "Niagara")
    class UNiagaraSystem* groundImpact01Niagara;

    UPROPERTY(EditAnywhere, Category = "Niagara")
    class UNiagaraSystem* groundImpactV2Niagara;

    UPROPERTY(EditAnywhere, Category = "Niagara")
    class UNiagaraSystem* groundImpactV3Niagara;

    UPROPERTY(EditAnywhere, Category = "Niagara")
    class UNiagaraSystem* groundImpactV4Niagara;

    UPROPERTY(EditAnywhere, Category = "Niagara")
    class UNiagaraSystem* groundImpactSwing;

    float currentTime;

    bool animOnce;

    bool animOnceV2;

    bool animOnceV3;

    bool animOnceV4;

    bool onceNiagara01 = false;

    bool onceNiagara02 = false;



    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector jumpAttack1CoolTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
    class UAnimMontage* jumpAttack01V1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
    class UAnimMontage* jumpAttack01V2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
    class UAnimMontage* jumpAttack01V3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
    class UAnimMontage* jumpAttack01V4;

    UPROPERTY(EditAnywhere, Category="MySettings")
    class APixelCodeCharacter* player;

    FVector playerLocation;

    bool jumpAttack1;
};
