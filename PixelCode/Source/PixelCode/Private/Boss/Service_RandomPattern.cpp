// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Service_RandomPattern.h"
#include "BehaviorTree/BlackboardComponent.h"

UService_RandomPattern::UService_RandomPattern()
{
	NodeName = TEXT("Random Pattern Maker");
}

void UService_RandomPattern::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	int32 value = FMath::RandRange(0, 10);
	if (value == 0)
	{
		
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if (value == 1)
	{
		
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if (value == 2)
	{
		
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if (value == 3)
	{
		
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if (value == 4)
	{
		
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if (value == 5)
	{
		
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if (value == 6)
	{
		
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if (value == 7)
	{
		
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if (value == 8)
	{
		
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if (value == 9)
	{
		
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else
	{
		
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}


	
}
