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
		UE_LOG(LogTemp, Warning, TEXT("Random0"));
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if (value == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Random1"));
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if (value == 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Random2"));
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if (value == 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("Random3"));
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if (value == 4)
	{
		UE_LOG(LogTemp, Warning, TEXT("Random4"));
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if (value == 5)
	{
		UE_LOG(LogTemp, Warning, TEXT("Random5"));
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if (value == 6)
	{
		UE_LOG(LogTemp, Warning, TEXT("Random6"));
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if (value == 7)
	{
		UE_LOG(LogTemp, Warning, TEXT("Random7"));
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if (value == 8)
	{
		UE_LOG(LogTemp, Warning, TEXT("Random8"));
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else if (value == 9)
	{
		UE_LOG(LogTemp, Warning, TEXT("Random9"));
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Random10"));
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), value);
	}


	
}
