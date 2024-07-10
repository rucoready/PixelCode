// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingWidget.h"
#include "Components/ScrollBox.h"
#include "CraftItemWidget.h"
#include "CustomTooltipWidget.h"
//#include <../../../../../../../Source/Runtime/Core/Public/UObject/WeakObjectPtrTemplates.h>
#include "Kismet/GameplayStatics.h"
#include "Player/PixelCodeCharacter.h"
#include "ItemStorage.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "Components/HorizontalBox.h"
#include "CraftingSlotWidget.h"
#include "Components/Button.h"



void UCraftingWidget::NativeConstruct()
{
	Super::NativeConstruct();


	bCraftable = true;
	SelectedIndex = 99;

	CraftList->ClearChildren();
	item_Recipes->ClearChildren();

	Btn_Craft->OnClicked.AddUniqueDynamic(this, &UCraftingWidget::OnCraftClicked);

	UE_LOG(LogTemp, Warning, TEXT("5555555555555"));
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
				MakeCraftItem(Index, GetItemNameFromType(Item.CraftedItem));
	
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
		const FText ItemName = GetItemNameFromType(EnumName);
		TextBlock_ItemName->SetText(ItemName);
		Image_Icon->SetBrush(GetItemIconFromType(EnumName));
		// 나중에 갯수 넣어줄 자리
		//T_Stack->SetText(FText::AsNumber(Crafts[SelectedIndex].CraftedItemAmount));
	}

	InitializeCraftSlot();
}



bool UCraftingWidget::isCraftable()
{
	if(!bCraftable)
	{
		return false;
	}
	/*else if(Char->isinventoryFull())
	{
		return false;
	}*/
	return true;
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

void UCraftingWidget::InitializeCraftSlot()
{
	item_Recipes->ClearChildren();

	bCraftable = true;

	FCraftItem Info = Crafts[SelectedIndex];
	for(FRecipe& Recipe : Info.CraftRecipes)
	{
		CreateCraftSlot(Recipe);
	}

}

void UCraftingWidget::OnCraftClicked()
{
	if(!Char)
	{
		return;
	}




	// 재료 사라짐
	if(isCraftable())
	{
		Char->CraftItem(Crafts[SelectedIndex]);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Craftable"));
	}


	// 플레이어 인벤토리 풀


	
	
}

void UCraftingWidget::CreateCraftSlot(const FRecipe& Recipe)
{
	if(!CraftingSlotTemplate)
	{
		return;
	}
	TWeakObjectPtr<UCraftingSlotWidget> CraftsSlot = CreateWidget<UCraftingSlotWidget>(this , CraftingSlotTemplate);
	if(CraftsSlot.IsValid())
	{
		CraftsSlot->SetData(FText::AsNumber(Recipe.Amount), GetItemIconFromType(Recipe.ItemType));
		//CraftsSlot->SetPadding(FMargin(20.f, 0.f));
		item_Recipes->AddChild(CraftsSlot.Get());

		if(CustomTooltipTemplate)
		{
		TWeakObjectPtr<UCustomTooltipWidget> Tooltip = CreateWidget<UCustomTooltipWidget>(this, CustomTooltipTemplate);

			if(Tooltip.IsValid())
			{ 
				Tooltip-> SetItemName(GetItemNameFromType(Recipe.ItemType));
				CraftsSlot->SetToolTip(Tooltip.Get());
			}
		}
			/*if(Char->getspecificItemAmount(Recipe.ItemType) >= Recipe.Amount)
			{
				CraftsSlot->SetBackgroundColorBase(true);

			}
			else
			{
				bCraftable = false;
				CraftsSlot->SetBackgroundColorBase(true);
			}*/


	}
}

FText UCraftingWidget::GetItemNameFromType(EItemName Name)
{
	return ItemStorage->GetCraftItemInfoBasedOn(Name).ItemName;
}

FSlateBrush UCraftingWidget::GetItemIconFromType(EItemName Name)
{
	return ItemStorage->GetCraftItemInfoBasedOn(Name).ItemIcon;
}

