// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API ABossAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	explicit ABossAIController(FObjectInitializer const& ObjectInitializer);

	virtual void OnPossess(APawn* InPawn)override;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UAISenseConfig_Sight* sightConfig;

	void UnPocessTree();

	class UBehaviorTreeComponent* BehaviorTreeComponent;
	class UBlackboardComponent* Blackboard;
};
