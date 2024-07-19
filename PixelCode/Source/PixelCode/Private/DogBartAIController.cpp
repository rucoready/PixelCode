// Fill out your copyright notice in the Description page of Project Settings.


#include "DogBartAIController.h"
#include "DogBart.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

ADogBartAIController::ADogBartAIController(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
}

void ADogBartAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (ADogBart* const dogBart = Cast<ADogBart>(InPawn))
    {
        if (UBehaviorTree* const Tree = dogBart->GetBehaviorTree())
        {
            BlackboardComp->InitializeBlackboard(*Tree->BlackboardAsset);
            BehaviorTreeComp->StartTree(*Tree);
        }
    }
}
