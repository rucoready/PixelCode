// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossAIController.h"
#include "Boss/TestBoss.h"
#include "Boss/BossApernia.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"




ABossAIController::ABossAIController(FObjectInitializer const& ObjectInitializer)
    : Super(ObjectInitializer)
{
    
	//SetupPerceptionSystem();
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
    
	if (ABossApernia* const bossApernia = Cast<ABossApernia>(InPawn))
	{
		if (UBehaviorTree* const tree = bossApernia->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);

		}
	}
}

void ABossAIController::UnPocessTree()
{
	

    
}



