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
    FBlackboardKeySelector forwardSlashCoolTime;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector doubleSwingAttackCoolTime;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector dodgeLeftCoolTime;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector dodgeRightCoolTime;

    class ABossApernia* boss;

    
    
    /// //////////////////////////////////////////////////////
    
    float currentTime_NormalAttack01 = 0.0f;
    float currentTime_NormalAttack02 = 0.0f;
    float currentTime_NormalAttack03 = 0.0f;
    float currentTime_JumpAttack1 = 0.0f;
    float currentTime_JumpAttack2 = 0.0f;
    float currentTime_JumpAttack3 = 0.0f;
    float currentTime_ForwardSlash = 0.0f;
    float currentTime_DoubleSwingAttack = 0.0f;
    float currentTime_DodgeLeft = 0.0f;
    float currentTime_DodgeRight = 0.0f;

    bool normalAttack01;
    bool normalAttack02;
    bool normalAttack03;
    bool jumpAttack1;
    bool jumpAttack2;
    bool jumpAttack3;
    bool forwardSlash;
    bool doubleSwingAttack;
    bool dodgeLeft;
    bool dodgeRight;
    
    bool coolTimeCounterHit;

    bool firstSkillCoolTimeSetting = false;
        
};
