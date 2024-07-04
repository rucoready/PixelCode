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
		CraftingWidget->SetCraftingInfo(ItemIndex);
	}
}

void UCraftItemWidget::SetData(uint16 Index, const FText& ItemName, TWeakObjectPtr<UCraftingWidget> Craft)
{
	ItemIndex = Index;
	CraftingWidget = Craft;
	T_CraftItemName->SetText(ItemName); 
}
