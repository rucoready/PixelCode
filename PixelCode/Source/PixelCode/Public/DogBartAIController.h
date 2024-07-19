// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DogBartAIController.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API ADogBartAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	explicit ADogBartAIController(FObjectInitializer const& ObjectInitializer);

	virtual void OnPossess(APawn* InPawn)override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BlackboardComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBehaviorTreeComponent* BehaviorTreeComp;
};
