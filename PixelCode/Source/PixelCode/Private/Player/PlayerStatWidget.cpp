// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerStatWidget.h"
#include "Player/StateComponent.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "Player/PixelCodeCharacter.h"
#include "GameFramework/PlayerController.h"




void UPlayerStatWidget::DisplayStat()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UPlayerStatWidget::HideStat()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerStatWidget::NativeConstruct()
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

		UpdateStat();
}

void UPlayerStatWidget::UpdateStat()
{	
	HP = FString::FromInt(Player->stateComp->MaxHP);  // float�� FString���� ��ȯ
	TB_HP->SetText(FText::FromString(HP));  // FString�� FText�� ��ȯ�Ͽ� UTextBlock�� ����

	MP = FString::FromInt(Player->stateComp->MaxMP);  // float�� FString���� ��ȯ
	TB_MP->SetText(FText::FromString(MP));  // FString�� FText�� ��ȯ�Ͽ� UTextBlock�� ����

	ATK = FString::FromInt(Player->stateComp->currentATK);  // float�� FString���� ��ȯ
	TB_ATK->SetText(FText::FromString(ATK));  // FString�� FText�� ��ȯ�Ͽ� UTextBlock�� ����

	DEF = FString::FromInt(Player->stateComp->currentDEF);  // float�� FString���� ��ȯ
	TB_DEF->SetText(FText::FromString(DEF));  // FString�� FText�� ��ȯ�Ͽ� UTextBlock�� ����

	CRITICAL = FString::FromInt(Player->stateComp->currentCritical);  // float�� FString���� ��ȯ
	TB_Critical->SetText(FText::FromString(CRITICAL));  // FString�� FText�� ��ȯ�Ͽ� UTextBlock�� ����

	STR = FString::FromInt(Player->stateComp->currentSTR);  // float�� FString���� ��ȯ
	TB_STR->SetText(FText::FromString(STR));  // FString�� FText�� ��ȯ�Ͽ� UTextBlock�� ����

	DEX = FString::FromInt(Player->stateComp->currentDex);  // float�� FString���� ��ȯ
	TB_DEX->SetText(FText::FromString(DEX));  // FString�� FText�� ��ȯ�Ͽ� UTextBlock�� ����

	INT = FString::FromInt(Player->stateComp->currentInt);  // float�� FString���� ��ȯ
	TB_INT->SetText(FText::FromString(INT));  // FString�� FText�� ��ȯ�Ͽ� UTextBlock�� ����

	LUCK = FString::FromInt(Player->stateComp->currentluck);  // float�� FString���� ��ȯ
	TB_LUCK->SetText(FText::FromString(LUCK));  // FString�� FText�� ��ȯ�Ͽ� UTextBlock�� ����

	CON = FString::FromInt(Player->stateComp->currentCon);  // float�� FString���� ��ȯ
	TB_CON->SetText(FText::FromString(CON));  // FString�� FText�� ��ȯ�Ͽ� UTextBlock�� ����
}






