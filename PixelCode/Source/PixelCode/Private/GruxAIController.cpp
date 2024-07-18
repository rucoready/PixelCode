// Fill out your copyright notice in the Description page of Project Settings.


#include "GruxAIController.h"
#include "Grux.h"
#include "GruxAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"



AGruxAIController::AGruxAIController(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
}

void AGruxAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (AGrux* const Grux = Cast<AGrux>(InPawn))
    {
        if (UBehaviorTree* const Tree = Grux->GetBehaviorTree())
        {
            BlackboardComp->InitializeBlackboard(*Tree->BlackboardAsset);
            BehaviorTreeComp->StartTree(*Tree);
        }
    }
}
