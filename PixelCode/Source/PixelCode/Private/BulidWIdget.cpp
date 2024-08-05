// Fill out your copyright notice in the Description page of Project Settings.


#include "BulidWIdget.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PixelCodeCharacter.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include "BuildItemSlot.h"
#include "ItemStorage.h"


void UBulidWIdget::NativeConstruct()
{

	Charar = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
}

void UBulidWIdget::SetBuildInfo(uint16 Index, uint16 ButtonIndex)
{

	if (BuildItemSlot.IsValidIndex(SelectedBuidIndex))
	{
		BuildItemSlot[SelectedBuidIndex]->ActivateBuildButton(false);
	}

	SelectedBuidIndex = ButtonIndex;
	SelectedBuilditemIndex = Index;

	if (BuildItemSlot.IsValidIndex(Index))
	{
		EItemName EnumName = BuildeCrafts[Index].CraftedItem;
		const FText ItemName = GetBuildNameFromType(EnumName);
		//T_MainText->SetText(ItemName);
		I_MainImage->SetBrush(GetBuildIconFromType(EnumName));
		// 나중에 갯수 넣어줄 자리
		//T_Stack->SetText(FText::AsNumber(Crafts[SelectedIndex].CraftedItemAmount));
	}


	//InitializeCraftSlot();
}

FText UBulidWIdget::GetBuildNameFromType(EItemName Name)
{
	return ItemStorage->GetCraftItemInfoBasedOn(Name).ItemName;
}

FSlateBrush UBulidWIdget::GetBuildIconFromType(EItemName Name)
{
	return ItemStorage->GetCraftItemInfoBasedOn(Name).ItemIcon;
}



