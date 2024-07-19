 // Fill out your copyright notice in the Description page of Project Settings.


#include "CraftItemWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include "CraftingWidget.h"

void UCraftItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button_CItem->OnClicked.AddUniqueDynamic(this,&UCraftItemWidget::OnCraftItemClicked);
}

void UCraftItemWidget::OnCraftItemClicked()
{
	if(CraftingWidget.IsValid())
	{
		CraftingWidget->SetCraftingInfo(ItemIndex, ButtonIndex);
		ActivateButton(true);
	}
}

void UCraftItemWidget::SetData(uint16 Index, uint16 InButtonIndex, const FText& ItemName, TWeakObjectPtr<UCraftingWidget> Craft)
{
	ItemIndex = Index;
	ButtonIndex = InButtonIndex;
	CraftingWidget = Craft;
	T_CraftItemName->SetText(ItemName); 
}

void UCraftItemWidget::ActivateButton(bool bActive)
{
	if(bActive)
	{
		Button_CItem->SetIsEnabled(false);
		Button_CItem->SetBackgroundColor(SelectedColor);
	}
	else
	{
		Button_CItem->SetIsEnabled(true);
		Button_CItem->SetBackgroundColor(DefalutColor);
	}
}
