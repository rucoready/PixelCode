// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Widget/NormallyWidget.h"
#include "Components/ProgressBar.h"
#include "Player/StateComponent.h"
#include "Player/pixelPlayerState.h"
#include "Player/PixelCodeCharacter.h"



void UNormallyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = nullptr;

	// GetOwningPlayer() 함수를 사용하여 Widget이 속한 Player를 가져옵니다.
	auto* OwningPlayer = GetOwningPlayer();
	if (OwningPlayer)
	{
		// OwningPlayer에서 MyCharacter로 다운캐스팅합니다.
		Player = Cast<APixelCodeCharacter>(OwningPlayer->GetPawn());
	}
	firstUpdate();
	
}

void UNormallyWidget::firstUpdate()
{
	
	PB_HP->SetPercent(Player->stateComp->MaxHP);
	PB_MP->SetPercent(Player->stateComp->MaxMP);
	UE_LOG(LogTemp, Warning, TEXT("PlayerStateNonull"));
	//PB_Exp->SetPercent(playerState->totalEXP);
	
	/*if (Player->stateComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerStatenull"));
	}*/

}


void UNormallyWidget::currentStatUpdate()
{
	PB_HP->SetPercent(Player->stateComp->currentHP/Player->stateComp->MaxHP);
	PB_MP->SetPercent(Player->stateComp->MaxMP/Player->stateComp->currentMP);
}

void UNormallyWidget::currentExpUpdate()
{
	PB_Exp->SetPercent(playerState->currentEXP/ playerState->totalEXP);
}



