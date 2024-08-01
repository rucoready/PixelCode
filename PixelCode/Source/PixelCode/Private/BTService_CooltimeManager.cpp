// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CooltimeManager.h"
#include "Player/PixelCodeCharacter.h" //캐릭터 임시
#include "Boss/BossApernia.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"

UBTService_CooltimeManager::UBTService_CooltimeManager()
{
    NodeName = TEXT("CoolTime Manager");
    
    
}

void UBTService_CooltimeManager::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!firstSkillCoolTimeSetting)
    {
        normalAttack01 = true;
        BlackboardComp->SetValueAsBool(normalAttack01CoolTime.SelectedKeyName, normalAttack01);
        normalAttack02 = true;
        BlackboardComp->SetValueAsBool(normalAttack02CoolTime.SelectedKeyName, normalAttack02);
        normalAttack03 = true;
        BlackboardComp->SetValueAsBool(normalAttack03CoolTime.SelectedKeyName, normalAttack03);
        jumpAttack1 = true;
        BlackboardComp->SetValueAsBool(jumpAttack1CoolTime.SelectedKeyName, jumpAttack1);
        jumpAttack2 = true;
        BlackboardComp->SetValueAsBool(jumpAttack2CoolTime.SelectedKeyName, jumpAttack2);
        jumpAttack3 = true;
        BlackboardComp->SetValueAsBool(jumpAttack3CoolTime.SelectedKeyName, jumpAttack3);
        forwardSlash = true;
        BlackboardComp->SetValueAsBool(forwardSlashCoolTime.SelectedKeyName, forwardSlash);
        doubleSwingAttack = true;
        BlackboardComp->SetValueAsBool(doubleSwingAttackCoolTime.SelectedKeyName, doubleSwingAttack);
        dodgeLeft = true;
        BlackboardComp->SetValueAsBool(dodgeLeftCoolTime.SelectedKeyName, dodgeLeft);
        dodgeRight = true;
        BlackboardComp->SetValueAsBool(dodgeRightCoolTime.SelectedKeyName, dodgeRight);

        firstSkillCoolTimeSetting = true;
    }
    //normalAttack 01 쿨타임

    //만약 블랙보드의 normalAttack01CoolTime이 false면  [ 10초 ]
    if (!BlackboardComp->GetValueAsBool(normalAttack01CoolTime.SelectedKeyName))
    {
        //normalAttack01에 10초의 쿨타임을 적용후 Set 을 True로 
        currentTime_NormalAttack01 += DeltaSeconds;
        if (currentTime_NormalAttack01 > 10.0f)
        {
            normalAttack01 = true;
            currentTime_NormalAttack01 = 0.0f;
            BlackboardComp->SetValueAsBool(normalAttack01CoolTime.SelectedKeyName, normalAttack01);
        }
        
    }

    //normalAttack 02 쿨타임 [ 10초 ]
    if (!BlackboardComp->GetValueAsBool(normalAttack02CoolTime.SelectedKeyName))
    {
        currentTime_NormalAttack02 += DeltaSeconds;
        if (currentTime_NormalAttack02 > 10.0f)
        {
            normalAttack02 = true;
            currentTime_NormalAttack02 = 0.0f;
            BlackboardComp->SetValueAsBool(normalAttack02CoolTime.SelectedKeyName, normalAttack02);
            
        }

    }

    //normalAttack 03 쿨타임 [ 20초 ]
    if (!BlackboardComp->GetValueAsBool(normalAttack03CoolTime.SelectedKeyName))
    {
        currentTime_NormalAttack03 += DeltaSeconds;
        if (currentTime_NormalAttack03 > 20.0f)
        {
            normalAttack03 = true;
            currentTime_NormalAttack03 = 0.0f;
            

            BlackboardComp->SetValueAsBool(normalAttack03CoolTime.SelectedKeyName, normalAttack03);
            
        }
        
        

    }

    //jumpAttack1 쿨타임 [30초]
    if (!BlackboardComp->GetValueAsBool(jumpAttack1CoolTime.SelectedKeyName))
    {
        currentTime_JumpAttack1 += DeltaSeconds;
        if (currentTime_JumpAttack1 > 30.0f)
        {
            jumpAttack1 = true;
            currentTime_JumpAttack1 = 0.0f;
            BlackboardComp->SetValueAsBool(jumpAttack1CoolTime.SelectedKeyName, jumpAttack1);
        }

    }

    //jumpAttack2 쿨타임 [30초]
    if (!BlackboardComp->GetValueAsBool(jumpAttack2CoolTime.SelectedKeyName))
    {
        currentTime_JumpAttack2 += DeltaSeconds;
        if (currentTime_JumpAttack2 > 30.0f)
        {
            jumpAttack2 = true;
            currentTime_JumpAttack2 = 0.0f;
            BlackboardComp->SetValueAsBool(jumpAttack2CoolTime.SelectedKeyName, jumpAttack2);
        }

    }

    //jumpAttack3 쿨타임 [10초]
    if (!BlackboardComp->GetValueAsBool(jumpAttack3CoolTime.SelectedKeyName))
    {
        currentTime_JumpAttack3 += DeltaSeconds;
        if (currentTime_JumpAttack3 > 10.0f)
        {
            jumpAttack3 = true;
            
            currentTime_JumpAttack3 = 0.0f;
            BlackboardComp->SetValueAsBool(jumpAttack3CoolTime.SelectedKeyName, jumpAttack3);
        }

    }

    //Forward Slash 쿨타임 [ 30초 ]
    if (!BlackboardComp->GetValueAsBool(forwardSlashCoolTime.SelectedKeyName))
    { 
        currentTime_ForwardSlash += DeltaSeconds;
        if (currentTime_ForwardSlash > 30.0f)
        {
            forwardSlash = true;
            currentTime_ForwardSlash = 0.0f;
            BlackboardComp->SetValueAsBool(forwardSlashCoolTime.SelectedKeyName, forwardSlash);
        }

    }

    //doubleSwingAttack [ 35초 ]
    if (!BlackboardComp->GetValueAsBool(doubleSwingAttackCoolTime.SelectedKeyName))
    {
        currentTime_DoubleSwingAttack += DeltaSeconds;
        if (currentTime_DoubleSwingAttack > 35.0f)
        {
            doubleSwingAttack = true;
            currentTime_DoubleSwingAttack = 0.0f;
            BlackboardComp->SetValueAsBool(doubleSwingAttackCoolTime.SelectedKeyName, doubleSwingAttack);
        }

    }

    //dodgeLeft [ 10초 ]
    if (!BlackboardComp->GetValueAsBool(dodgeLeftCoolTime.SelectedKeyName))
    {
        currentTime_DodgeLeft += DeltaSeconds;
        if (currentTime_DodgeLeft > 10.0f)
        {
            dodgeLeft = true;
            currentTime_DodgeLeft = 0.0f;
            BlackboardComp->SetValueAsBool(dodgeLeftCoolTime.SelectedKeyName, dodgeLeft);
        }

    }

    //dodgeRight [ 10초 ]
    if (!BlackboardComp->GetValueAsBool(dodgeRightCoolTime.SelectedKeyName))
    {
        currentTime_DodgeRight += DeltaSeconds;
        if (currentTime_DodgeRight > 10.0f)
        {
            dodgeRight = true;
            currentTime_DodgeRight = 0.0f;
            BlackboardComp->SetValueAsBool(dodgeRightCoolTime.SelectedKeyName, dodgeRight);
        }

    }

    if (boss)
    {
        if (boss->bossHitCounterAttack == true)
        {
            BlackboardComp->SetValueAsBool(jumpAttack2CoolTime.SelectedKeyName, false);
        }
    }

    if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
    {

        APawn* ControlledPawn = bossController->GetPawn();
        if (ControlledPawn)
        {
            
            boss = Cast<ABossApernia>(ControlledPawn);
            if (boss->TaskCheckCounterHit == true)
            {
                coolTimeCounterHit = false;
                UE_LOG(LogTemp, Warning, TEXT("III999"));
                BlackboardComp->SetValueAsBool(jumpAttack2CoolTime.SelectedKeyName, coolTimeCounterHit);
                BlackboardComp->SetValueAsBool(normalAttack02CoolTime.SelectedKeyName, coolTimeCounterHit);
            }
            
        }
    }
    
}
