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
		
		// GetOwningPlayer() 함수를 사용하여 Widget이 속한 Player를 가져옵니다.
		auto* OwningPlayer = GetOwningPlayer();
		if (OwningPlayer)
		{
			// OwningPlayer에서 MyCharacter로 다운캐스팅합니다.
			Player = Cast<APixelCodeCharacter>(OwningPlayer->GetPawn());
		}

		UpdateStat();
}

void UPlayerStatWidget::UpdateStat()
{	
	HP = FString::FromInt(Player->stateComp->MaxHP);  // float을 FString으로 변환
	TB_HP->SetText(FText::FromString(HP));  // FString을 FText로 변환하여 UTextBlock에 설정

	MP = FString::FromInt(Player->stateComp->MaxMP);  // float을 FString으로 변환
	TB_MP->SetText(FText::FromString(MP));  // FString을 FText로 변환하여 UTextBlock에 설정

	ATK = FString::FromInt(Player->stateComp->currentATK);  // float을 FString으로 변환
	TB_ATK->SetText(FText::FromString(ATK));  // FString을 FText로 변환하여 UTextBlock에 설정

	DEF = FString::FromInt(Player->stateComp->currentDEF);  // float을 FString으로 변환
	TB_DEF->SetText(FText::FromString(DEF));  // FString을 FText로 변환하여 UTextBlock에 설정

	CRITICAL = FString::FromInt(Player->stateComp->currentCritical);  // float을 FString으로 변환
	TB_Critical->SetText(FText::FromString(CRITICAL));  // FString을 FText로 변환하여 UTextBlock에 설정

	STR = FString::FromInt(Player->stateComp->currentSTR);  // float을 FString으로 변환
	TB_STR->SetText(FText::FromString(STR));  // FString을 FText로 변환하여 UTextBlock에 설정

	DEX = FString::FromInt(Player->stateComp->currentDex);  // float을 FString으로 변환
	TB_DEX->SetText(FText::FromString(DEX));  // FString을 FText로 변환하여 UTextBlock에 설정

	INT = FString::FromInt(Player->stateComp->currentInt);  // float을 FString으로 변환
	TB_INT->SetText(FText::FromString(INT));  // FString을 FText로 변환하여 UTextBlock에 설정

	LUCK = FString::FromInt(Player->stateComp->currentluck);  // float을 FString으로 변환
	TB_LUCK->SetText(FText::FromString(LUCK));  // FString을 FText로 변환하여 UTextBlock에 설정

	CON = FString::FromInt(Player->stateComp->currentCon);  // float을 FString으로 변환
	TB_CON->SetText(FText::FromString(CON));  // FString을 FText로 변환하여 UTextBlock에 설정
}






