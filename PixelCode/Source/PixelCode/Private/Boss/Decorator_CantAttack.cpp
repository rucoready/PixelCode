// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Decorator_CantAttack.h"
#include "BehaviorTree/BlackboardComponent.h"

UDecorator_CantAttack::UDecorator_CantAttack()
{
	NodeName = TEXT("Check Can Attack");

    bNotifyTick = true;
}

bool UDecorator_CantAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    return true;

    

    
}

void UDecorator_CantAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    // 블랙보드 키로부터 값을 가져옴
    bool value = OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());

    
}
