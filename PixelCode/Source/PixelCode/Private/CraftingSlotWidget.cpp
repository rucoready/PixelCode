// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingSlotWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>


void UCraftingSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();


}

void UCraftingSlotWidget::SetBackgroundColorBase(bool bAvailable)
{
	if(bAvailable)
	{
		I_BorderColor->SetColorAndOpacity(AvailableColor);
	}
	else
	{
		I_BorderColor->SetColorAndOpacity(NotAvailableColor);
	}
	

}

void UCraftingSlotWidget::SetData(const FText& Amount, const FSlateBrush& Icon)
{
	 T_ItemAmount->SetText(Amount);
	 I_ItemIcon->SetBrush(Icon);
}
