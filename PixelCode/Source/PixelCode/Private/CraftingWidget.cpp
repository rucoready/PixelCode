// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingWidget.h"
#include "Components/ScrollBox.h"
#include "CraftItemWidget.h"
#include <../../../../../../../Source/Runtime/Core/Public/UObject/WeakObjectPtrTemplates.h>
#include "Kismet/GameplayStatics.h"
#include "Player/PixelCodeCharacter.h"
#include "ItemStorage.h"



void UCraftingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CraftList->ClearChildren();
	MakeCraftItem(0,FText::FromString("Test Item"));
	MakeCraftItem(1,FText::FromString("Test Item2"));

	Char = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(Char)
	{
		ItemStorage = Char->GetItemStorage();
		if(ItemStorage)
		{
			// gett all recipe
			//ItemStorage->GetCraftItemInfoBasedOn(EItemName::ETN_WoodFoundation);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemStorage->GetCraftItemInfoBasedOn(EItemName::ETN_WoodFoundation).ItemName.ToString());
		}
	}

}

void UCraftingWidget::MakeCraftItem(uint8 Index, const FText& ItemName)
{
	if(CraftItemClass)
	{
		TWeakObjectPtr<UCraftItemWidget> CrafgSlot = CreateWidget<UCraftItemWidget>(this, CraftItemClass);
		if(CrafgSlot->IsValidLowLevel())
		{
			CrafgSlot->SetData(Index,ItemName);
			CrafgSlot->SetPadding(FMargin(0.f,10.f, 0.f, 0.f));
			CraftItems.Add(CrafgSlot);
			CraftList->AddChild(CrafgSlot.Get());
		}
	}
}
