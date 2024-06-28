// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Delegates/Delegate.h"
#include "Task_BossAttackC1.generated.h"



/**
 * 
 */
UCLASS()
class PIXELCODE_API UTask_BossAttackC1 : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	explicit UTask_BossAttackC1(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* swordComboAttack1;

	
};
