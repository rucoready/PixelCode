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

    currentTime += DeltaSeconds;
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    //normalAttack 01 쿨타임

    //만약 블랙보드의 normalAttack01CoolTime이 false면  [ 10초 ]
    if (!BlackboardComp->GetValueAsBool(normalAttack01CoolTime.SelectedKeyName))
    {
        //normalAttack01에 10초의 쿨타임을 적용후 Set 을 True로 
        if (currentTime > 10.0f)
        {
            normalAttack01 = true;
        }
        
    }

    //normalAttack 02 쿨타임 [ 10초 ]
    if (!BlackboardComp->GetValueAsBool(normalAttack02CoolTime.SelectedKeyName))
    {
        if (currentTime > 10.0f)
        {
            normalAttack02 = true;
        }

    }

    //normalAttack 03 쿨타임 [ 10초 ]
    if (!BlackboardComp->GetValueAsBool(normalAttack03CoolTime.SelectedKeyName))
    {
        if (currentTime > 10.0f)
        {
            normalAttack03 = true;
        }

    }

    //jumpAttack2 쿨타임
    if (!BlackboardComp->GetValueAsBool(jumpAttack1CoolTime.SelectedKeyName))
    {
        //normalAttack01에 10초의 쿨타임을 적용후 Set 을 True로 
        if (currentTime > 10.0f)
        {
            jumpAttack1 = true;
        }

    }

    //jumpAttack2 쿨타임
    if (!BlackboardComp->GetValueAsBool(jumpAttack2CoolTime.SelectedKeyName))
    {
        //normalAttack01에 10초의 쿨타임을 적용후 Set 을 True로 
        if (currentTime > 10.0f)
        {
            jumpAttack2 = true;
        }

    }

    //jumpAttack3 쿨타임
    if (!BlackboardComp->GetValueAsBool(jumpAttack3CoolTime.SelectedKeyName))
    {
        //normalAttack01에 10초의 쿨타임을 적용후 Set 을 True로 
        if (currentTime > 10.0f)
        {
            jumpAttack3 = true;
        }

    }

    //comboAttack 쿨타임
    if (!BlackboardComp->GetValueAsBool(comboAttackCoolTime.SelectedKeyName))
    {
        //normalAttack01에 10초의 쿨타임을 적용후 Set 을 True로 
        if (currentTime > 10.0f)
        {
            comboAttack = true;
        }

    }

    //doubleSwingAttack 쿨타임
    if (!BlackboardComp->GetValueAsBool(doubleSwingAttackCoolTime.SelectedKeyName))
    {
        //normalAttack01에 10초의 쿨타임을 적용후 Set 을 True로 
        if (currentTime > 10.0f)
        {
            doubleSwingAttack = true;
        }

    }
    
    


    BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsBool(normalAttack01CoolTime.SelectedKeyName, normalAttack01);
        BlackboardComp->SetValueAsBool(normalAttack02CoolTime.SelectedKeyName, false);
        BlackboardComp->SetValueAsBool(normalAttack03CoolTime.SelectedKeyName, false);
        BlackboardComp->SetValueAsBool(jumpAttack1CoolTime.SelectedKeyName, false);
        BlackboardComp->SetValueAsBool(jumpAttack2CoolTime.SelectedKeyName, false);
        BlackboardComp->SetValueAsBool(jumpAttack3CoolTime.SelectedKeyName, false);
        BlackboardComp->SetValueAsBool(comboAttackCoolTime.SelectedKeyName, false);
        BlackboardComp->SetValueAsBool(doubleSwingAttackCoolTime.SelectedKeyName, false);
    }
}
