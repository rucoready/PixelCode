// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_SpecialAttack01.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UTask_SpecialAttack01 : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	explicit UTask_SpecialAttack01(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	

	FVector playerLocation;

	bool animOnce;

	bool animOnceV2;

	bool animOnceV3;

	float currentTime;

	bool jumpOnce = false;

	bool jumpOnce2 = false;

	bool jumpNiagara1 = false;

	bool jumpNiagara2 = false;

	bool jumpNiagara3 = false;


	UPROPERTY(EditAnywhere, Category = "Niagara")
	class UNiagaraSystem* jumpSwing;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	class UNiagaraSystem* jumpSwing2;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	class UNiagaraSystem* jumpSwing3;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector jumpAttack2CoolTime;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class APixelCodeCharacter* player;

	bool jumpAttack2;
};
