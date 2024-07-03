// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_CheckHitByPlayer.h"

UService_CheckHitByPlayer::UService_CheckHitByPlayer()
{
	NodeName = TEXT("Check Hit By Player");
	
}

void UService_CheckHitByPlayer::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	
}

void UService_CheckHitByPlayer::OnBeginOverlapPlyerKatana(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
