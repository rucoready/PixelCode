// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomTooltipWidget.h"
#include "Components/TextBlock.h"



void UCustomTooltipWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCustomTooltipWidget::SetItemName(const FText& Name)
{
	T_ItemName->SetText(Name);
}
