// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingWidget.h"
#include "Components/ScrollBox.h"
#include "CraftItemWidget.h"
//#include <../../../../../../../Source/Runtime/Core/Public/UObject/WeakObjectPtrTemplates.h>
#include "Kismet/GameplayStatics.h"
#include "Player/PixelCodeCharacter.h"
#include "ItemStorage.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>



void UCraftingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SelectedIndex = 99;

	CraftList->ClearChildren();
	UE_LOG(LogTemp, Warning, TEXT("5555555555555"))
	//MakeCraftItem(1, FText::FromString("test item"));
	//UPROPERTY(EditAnywhere, Category = "Item")MakeCraftItem(2, FText::FromString("test item"));

	Char = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(Char)
	{
		ItemStorage = Char->GetItemStorage();
		if(ItemStorage)
		{
			uint8 Index = 0;
			
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemStorage->GetCraftItemInfoBasedOn(EItemName::EIN_Wood).ItemName.ToString());
			Crafts = ItemStorage->GetAllCrafting();
			for (FCraftItem& Item : Crafts)
			{
				MakeCraftItem(Index, ItemStorage->GetCraftItemInfoBasedOn(Item.CraftedItem).ItemName);
	
				if(Index == 0)
				{
					SetCraftingInfo(0);
					if(CraftItems.IsValidIndex(0))
					{
						CraftItems[0]->ActivateButton(true);
					}
				}
				Index++;
				//UE_LOG(LogTemp, Warning, TEXT("3333333333"))
				//UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemStorage->GetCraftItemInfoBasedOn(Item.CraftedItem).ItemName.ToString());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("item storage is invalid"))
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player character is invalid"))
	}
}

void UCraftingWidget::SetCraftingInfo(uint8 Index)
{

	if(CraftItems.IsValidIndex(SelectedIndex))
	{
		CraftItems[SelectedIndex]->ActivateButton(false);
	}
	
	SelectedIndex = Index;
	if(CraftItems.IsValidIndex(SelectedIndex))
	{ 
		EItemName EnumName = Crafts[SelectedIndex].CraftedItem;
		const FText ItemName = ItemStorage->GetCraftItemInfoBasedOn(EnumName).ItemName;
		TextBlock_ItemName->SetText(ItemName);
		Image_Icon->SetBrush(ItemStorage->GetCraftItemInfoBasedOn(EnumName).ItemIcon);
		// 나중에 갯수 넣어줄 자리
		//T_Stack->SetText(FText::AsNumber(Crafts[SelectedIndex].CraftedItemAmount));
	}
}

void UCraftingWidget::MakeCraftItem(uint16 Index, const FText& ItemName)
{
	if(CraftItemTemplate)
	{
		TWeakObjectPtr<UCraftItemWidget> CraftSlot = CreateWidget<UCraftItemWidget>(this, CraftItemTemplate);
		if(CraftSlot.IsValid())
		{
			CraftSlot->SetData(Index,ItemName, this);
			CraftSlot->SetPadding(FMargin(0.f,10.f, 0.f, 0.f));
			CraftItems.Add(CraftSlot);
			CraftList->AddChild(CraftSlot.Get());
		}
	}
}
