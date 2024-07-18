// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_GruxPatrol.h"
#include "GruxAIController.h"
#include "NavigationPath.h"
#include "AI/Navigation/NavigationTypes.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"

UTask_GruxPatrol::UTask_GruxPatrol(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Grux Patrol");


	bNotifyTick = true;

}

EBTNodeResult::Type UTask_GruxPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TickTask(OwnerComp, NodeMemory, 0.0f);
	AGruxAIController* gruxController = Cast<AGruxAIController>(OwnerComp.GetAIOwner());

	if (gruxController)
	{
		FVector origin = gruxController->GetPawn()->GetActorLocation();
		navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
		FVector randomLocation;

		if (navSys)
		{
			if (navSys->K2_GetRandomReachablePointInRadius(GetWorld(), origin, randomLocation, 1500.0))
			{
				gruxController->MoveToLocation(randomLocation);
				
			}

		}
	}
	return EBTNodeResult::Succeeded;
}

void UTask_GruxPatrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	currentTime += DeltaSeconds;

	
}
