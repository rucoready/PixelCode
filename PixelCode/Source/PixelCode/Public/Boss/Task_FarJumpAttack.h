// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_FarJumpAttack.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UTask_FarJumpAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
    UTask_FarJumpAttack(FObjectInitializer const& ObjectInitializer);

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    float currentTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
    class UAnimMontage* farStompAttack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
    TSubclassOf<UCameraShakeBase> cameraShakeOBJ;

    UPROPERTY(EditAnywhere, Category = "Particles")
    UParticleSystem* groundImpactParticle;

    UPROPERTY(EditAnywhere, Category = "Niagara")
    class UNiagaraSystem* groundImpactNiagara;

    FVector previousPlayerLocation;

    bool animOnce;

    bool moveToPlayerOnce = false;

    bool onceCameraShake = false;

    bool onceSpawnGroundParticle01 = false;

    FVector playerLocation;

    float speed = 4000.0f;

    float speedAddMovement = 3200.0f;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    class APixelCodeCharacter* player;
};
