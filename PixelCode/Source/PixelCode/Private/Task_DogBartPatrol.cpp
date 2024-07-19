// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_DogBartPatrol.h"
#include "DogBartAIController.h"
#include "NavigationPath.h"
#include "AI/Navigation/NavigationTypes.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"

UTask_DogBartPatrol::UTask_DogBartPatrol(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("DogBart Patrol");


	bNotifyTick = true;
}

EBTNodeResult::Type UTask_DogBartPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TickTask(OwnerComp, NodeMemory, 0.0f);
	ADogBartAIController* dogBartController = Cast<ADogBartAIController>(OwnerComp.GetAIOwner());

	if (dogBartController)
	{
		FVector origin = dogBartController->GetPawn()->GetActorLocation();
		navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
		FVector randomLocation;

		if (navSys)
		{
			if (navSys->K2_GetRandomReachablePointInRadius(GetWorld(), origin, randomLocation, 1500.0))
			{
				dogBartController->MoveToLocation(randomLocation);

			}

		}
	}
	return EBTNodeResult::Succeeded;
}

void UTask_DogBartPatrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	currentTime += DeltaSeconds;
}
