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

	// GetOwningPlayer() �Լ��� ����Ͽ� Widget�� ���� Player�� �����ɴϴ�.
	auto* OwningPlayer = GetOwningPlayer();
	if (OwningPlayer)
	{
		// OwningPlayer���� MyCharacter�� �ٿ�ĳ�����մϴ�.
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



