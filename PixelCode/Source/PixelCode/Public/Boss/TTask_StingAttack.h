// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "TTask_StingAttack.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UTTask_StingAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
    UTTask_StingAttack(FObjectInitializer const& ObjectInitializer);

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
    class UAnimMontage* stingAttack01;

    bool animOnce;

    float currentTime;

    float speed = 720.0f;

    float acceleration;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector playerLocationBlackboardKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
    TSubclassOf<UCameraShakeBase> cameraShakeOBJ;

    UPROPERTY(EditAnywhere, Category = "Niagara")
    class UNiagaraSystem* stingAttackNiagara;

    float currentSpeed;

    bool bDirectionSet;

    bool onceCameraShake = false;

    bool onceSpawnStingNiagara = false;

    FVector playerLocation;

    FVector moveDirection;
    FVector lastValidLocation;
    FVector savedTargetLocation;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    class APixelCodeCharacter* player;

};
