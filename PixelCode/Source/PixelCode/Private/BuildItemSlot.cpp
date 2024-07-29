// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildItemSlot.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include "BulidWIdget.h"

void UBuildItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	//Btn_BuildButton->OnClicked.AddUniqueDynamic(this, &UBulidWidget::OnCraftItemClicked);

}

void UBuildItemSlot::SetData(uint16 Index, uint16 BuildIndexs, const FText& BItemName, const FSlateBrush& Icon, TWeakObjectPtr<UBulidWIdget> Build)
{
	BuildIndex = Index;
	ButtonBuildIndex = BuildIndexs;
	BuildWidget = Build;
	T_BuildItemName->SetText(BItemName);
	I_BuildImage->SetBrush(Icon);
}

void UBuildItemSlot::OnBuildItemClicked()
{
	if (BuildWidget.IsValid())
	{
		BuildWidget->SetBuildInfo(BuildIndex, ButtonBuildIndex);
		ActivateBuildButton(true);
	}
}



void UBuildItemSlot::ActivateBuildButton(bool bBActive)
{
	if (bBActive)
	{
		Btn_BuildButton->SetIsEnabled(false);
		Btn_BuildButton->SetBackgroundColor(SelectedBuildColor);
	}
	else
	{
		Btn_BuildButton->SetIsEnabled(true);
		Btn_BuildButton->SetBackgroundColor(DefalutBuildColor);
	}
}
