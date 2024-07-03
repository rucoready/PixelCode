// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Service_CheckHitByPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UService_CheckHitByPlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UService_CheckHitByPlayer();


	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	UFUNCTION()
	void OnBeginOverlapPlyerKatana(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
