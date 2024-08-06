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
#include "Player/World/Pickup.h"
#include "Components/Button.h"
#include "Player/inventory/InventoryComponent.h"



void UCraftingWidget::NativeConstruct()
{
	Super::NativeConstruct();


	bCraftable = true;



	Btn_Craft->OnClicked.AddUniqueDynamic(this, &UCraftingWidget::OnCraftClicked);
	Btn_Craft->OnHovered.AddUniqueDynamic(this, &UCraftingWidget::OnCraftHoverd);
	Btn_Craft->OnUnhovered.AddUniqueDynamic(this, &UCraftingWidget::OnCraftUnHoverd);


	
	//MakeCraftItem(1, FText::FromString("test item"));
	//UPROPERTY(EditAnywhere, Category = "Item")MakeCraftItem(2, FText::FromString("test item"));
	

	Char = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	RefreshCraftItem();
}

void UCraftingWidget::SetCraftingInfo(uint16 Index, uint16 ButtonIndex)
{

	if(CraftItems.IsValidIndex(SelectedIndex))
	{
		CraftItems[SelectedIndex]->ActivateButton(false);
	}
	
	SelectedIndex = ButtonIndex;
	SelectedItemIndex = Index;

	if(CraftItems.IsValidIndex(Index))
	{ 
		EItemName EnumName = Crafts[Index].CraftedItem;
		const FText ItemName = GetItemNameFromType(EnumName);
		TextBlock_ItemName->SetText(ItemName);
		Image_Icon->SetBrush(GetItemIconFromType(EnumName));
		// 나중에 갯수 넣어줄 자리
		//T_Stack->SetText(FText::AsNumber(Crafts[SelectedIndex].CraftedItemAmount));
	}


	InitializeCraftSlot();
}


// 크래프트 또는 실패 bool 값
bool UCraftingWidget::isCraftable()
{
	if(!bCraftable)
	{
		return false;
	}
	//else if(FItemAddResult infAddedAll())
	//{
	////(Char->isinventoryFull())
	//	return false;
	//}
	return true;
}

void UCraftingWidget::MakeCraftItem(uint16 Index,uint16 ButtonIndex, const FText& ItemName)
{    
	if(CraftItemTemplate)
	{
		TWeakObjectPtr<UCraftItemWidget> CraftSlot = CreateWidget<UCraftItemWidget>(this, CraftItemTemplate);
		if(CraftSlot.IsValid())
		{
			CraftSlot->SetData(Index, ButtonIndex ,ItemName, this);
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
		CreateCraftRecipeSlot(Recipe);
	}

}

void UCraftingWidget::RefreshCraftItem()
{
	CraftList->ClearChildren(); 

	if (Char)
	{
		ItemStorage = Char->GetItemStorage();
		if (ItemStorage)
		{
			uint8 Index = 0;
			uint16 ButtonIndex = 0;
			Crafts.Empty();
			CraftItems.Empty();

			//UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemStorage->GetCraftItemInfoBasedOn(EItemName::EIN_Wood).ItemName.ToString());
			Crafts = ItemStorage->GetAllCrafting();
			for (FCraftItem& Item : Crafts)
			{
				if (Char->IsPlayerInCraftArea(Item.CraftCondition))
				{
					Index++;
					continue;
				}

				MakeCraftItem(Index,ButtonIndex, GetItemNameFromType(Item.CraftedItem));

				if (Index == 0)
				{
					SetCraftingInfo(0, 0);
					if (CraftItems.IsValidIndex(0))
					{
						CraftItems[0]->ActivateButton(true);
					}
				}
				Index++;
				ButtonIndex++;
				//UE_LOG(LogTemp, Warning, TEXT("3333333333"))
				//UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemStorage->GetCraftItemInfoBasedOn(Item.CraftedItem).ItemName.ToString());
			}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("item storage is invalid"))
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player character is invalid"))
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
		if(CraftSuccessSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), CraftSuccessSound);
		}

		//UE_LOG(LogTemp, Warning, TEXT("CLICK"));
		auto Pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (Pc->HasAuthority())
		{
			Char->CraftItem(Crafts[SelectedIndex]);
			InitializeCraftSlot();
		}
		else
		{
			Char->ServerRPC_CraftItem(Crafts[SelectedIndex]);
			InitializeCraftSlot();
		}
	}
	else
	{
		//UE_LOG(LogTemp,Warning,TEXT("Craftable"));
	}


	// 플레이어 인벤토리 풀


	
	
}

void UCraftingWidget::OnCraftHoverd()
{
	bBulkCraft =true;
}

void UCraftingWidget::OnCraftUnHoverd()
{
	bBulkCraft = false;
}

void UCraftingWidget::OnCraftbulkPressed()
{
}

void UCraftingWidget::RefreshCraftingScreen()
{
	SelectedIndex = 99;
	CraftList->ClearChildren();
	item_Recipes->ClearChildren();

	RefreshCraftItem();
}

void UCraftingWidget::CreateCraftRecipeSlot(const FRecipe& Recipe)
{
	if(!CraftingSlotTemplate || !Char)
	{
		return;
	}

	

	TWeakObjectPtr<UCraftingSlotWidget> CraftsSlot = CreateWidget<UCraftingSlotWidget>(this , CraftingSlotTemplate);
	if(CraftsSlot.IsValid())
	{
		CraftsSlot->SetData(FText::AsNumber(Recipe.Amount), GetItemIconFromType(Recipe.ItemType));
		//CraftsSlot->SetPadding(FMargin(10.f, 0.f, 10.f, 0.f));
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
			if(Char->GetSpecificItemAmount(Recipe.ItemType) >= Recipe.Amount) // 재료의 필요 개수보다 재료 아이템이 같거나 많다면 초록색으로 변경되게
			{
				// 색상 변경
				CraftsSlot->SetBackgroundColorBase(true);
				//UE_LOG(LogTemp, Warning, TEXT("color green"));
			}
			else
			{
				// 색상 변경
				bCraftable = false;
				CraftsSlot->SetBackgroundColorBase(false); // 그렇지 않다면 빨간색 기본
				//UE_LOG(LogTemp, Warning, TEXT("color red"));
			}


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

