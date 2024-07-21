// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerStatWidget.h"
#include "Player/StateComponent.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "Player/PixelCodeCharacter.h"
#include "PCodePlayerController.h"
#include "Player/pixelPlayerState.h"
#include "MyGameModeBase.h"




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

	//auto* GM = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
	// GetOwningPlayer() 함수를 사용하여 Widget이 속한 Player를 가져옵니다.
	auto* OwningPlayer = GetOwningPlayer();
	if (OwningPlayer)
	{
		// OwningPlayer에서 MyCharacter로 다운캐스팅합니다.
		Player = Cast<APixelCodeCharacter>(OwningPlayer->GetPawn());
		if (Player != nullptr)
		{
			PlayerState = Cast<ApixelPlayerState>(Player->PlayerState);
		}
	}

	
	

	UpdateStat();
}

void UPlayerStatWidget::UpdateStat()
{
	LEVEL = FString::FromInt(PlayerState->Level);  // float을 FString으로 변환
	TB_LEVEL->SetText(FText::FromString(LEVEL));  // FString을 FText로 변환하여 UTextBlock에 설정

	HP = FString::FromInt(Player->stateComp->MaxHP); 
	TB_HP->SetText(FText::FromString(HP));  

	MP = FString::FromInt(Player->stateComp->MaxMP);  
	TB_MP->SetText(FText::FromString(MP));  

	ATK = FString::FromInt(Player->stateComp->currentATK); 
	TB_ATK->SetText(FText::FromString(ATK));  

	DEF = FString::FromInt(Player->stateComp->currentDEF); 
	TB_DEF->SetText(FText::FromString(DEF)); 

	CRITICAL = FString::FromInt(Player->stateComp->currentCritical);  
	TB_Critical->SetText(FText::FromString(CRITICAL)); 

	STR = FString::FromInt(Player->stateComp->currentSTR);  
	TB_STR->SetText(FText::FromString(STR));  

	DEX = FString::FromInt(Player->stateComp->currentDex); 
	TB_DEX->SetText(FText::FromString(DEX)); 

	INT = FString::FromInt(Player->stateComp->currentInt); 
	TB_INT->SetText(FText::FromString(INT));  

	LUCK = FString::FromInt(Player->stateComp->currentluck);  
	TB_LUCK->SetText(FText::FromString(LUCK)); 

	CON = FString::FromInt(Player->stateComp->currentCon); 
	TB_CON->SetText(FText::FromString(CON));  
}





