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

	CraftList->ClearChildren();
	UE_LOG(LogTemp, Warning, TEXT("5555555555555"))


	Char = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ItemStorage = Cast<AItemStorage>(this);
	if(Char)
	{
		//ItemStorage = Char->GetItemStorage();
		if(ItemStorage)
		{
			//uint8 Index = 0;
			
			UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemStorage->GetCraftItemInfoBasedOn(EItemName::EIN_Wood).ItemName.ToString());
			//Crafts = ItemStorage->GetAllCrafting();
			//for (FCraftItem& Item : Crafts)
			//{
			//	MakeCraftItem(Index, ItemStorage->GetCraftItemInfoBasedOn(Item.CraftedItem).ItemName);
			//	Index++;
			//	UE_LOG(LogTemp, Warning, TEXT("3333333333"))
			//	//UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemStorage->GetCraftItemInfoBasedOn(Item.CraftedItem).ItemName.ToString());
			//}
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
	SelectedIndex = Index;
	if(CraftItems.IsValidIndex(SelectedIndex))
	{ 
		const FText ItemName = ItemStorage->GetCraftItemInfoBasedOn(Crafts[SelectedIndex].CraftedItem).ItemName;
		TextBlock_ItemName->SetText(ItemName);
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
