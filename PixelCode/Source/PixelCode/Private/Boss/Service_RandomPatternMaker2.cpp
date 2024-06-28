// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Service_RandomPatternMaker2.h"
#include "BehaviorTree/BlackboardComponent.h"

UService_RandomPatternMaker2::UService_RandomPatternMaker2()
{
	NodeName = TEXT("Random Pattern Maker2");
}

void UService_RandomPatternMaker2::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	int32 value = FMath::RandRange(0, 3);
	if (value == 0)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if(value == 1)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if (value == 2)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
}
