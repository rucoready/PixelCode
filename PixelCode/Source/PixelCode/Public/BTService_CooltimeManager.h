// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CooltimeManager.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UBTService_CooltimeManager : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_CooltimeManager();


	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    // Blackboard keys for different boolean variables
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector normalAttack01CoolTime;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector normalAttack02CoolTime;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector normalAttack03CoolTime;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector jumpAttack1CoolTime;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector jumpAttack2CoolTime;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector jumpAttack3CoolTime;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector comboAttackCoolTime;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector doubleSwingAttackCoolTime;

    float currentTime;

    bool normalAttack01 = true;
    bool normalAttack02 = true;
    bool normalAttack03 = true;
    bool jumpAttack1 = true;
    bool jumpAttack2 = true;
    bool jumpAttack3 = true;
    bool comboAttack = true;
    bool doubleSwingAttack = true;
        
};
