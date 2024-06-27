// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerStatWidget.h"
#include "Player/StateComponent.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>




void UPlayerStatWidget::DisplayStat()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UPlayerStatWidget::HideStat()
{
	SetVisibility(ESlateVisibility::Collapsed);
}



void UPlayerStatWidget::UpdateStat()
{

	FString HPString = FString::Printf(TEXT("%d"), statComp->currentHP);
	FText HPText = FText::FromString(HPString);
	TB_HP->SetText(HPText);
	UE_LOG(LogTemp, Warning, TEXT("stat"));

}


