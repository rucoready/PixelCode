// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task_BossAttackC1.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Boss/BossAIController.h"
#include "Boss/BossApernia.h"

UTask_BossAttackC1::UTask_BossAttackC1(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Combo Attack 01");
    

    static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageObj(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationFinish/AM_BossComboAttack01.AM_BossComboAttack01'"));
    if (MontageObj.Succeeded())
    {
        swordComboAttack1 = MontageObj.Object;
    }
}

EBTNodeResult::Type UTask_BossAttackC1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (AAIController* bossController = Cast<AAIController>(OwnerComp.GetOwner()))
    {
        APawn* ControlledPawn = bossController->GetPawn();
        if (ControlledPawn)
        {
            ACharacter* boss = Cast<ACharacter>(ControlledPawn);

            if (swordComboAttack1 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance())
            {
                //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance(); 
                
                boss->PlayAnimMontage(swordComboAttack1);        

            }
        }
    }
    return EBTNodeResult::Succeeded;

}


