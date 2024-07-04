// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CreateItemData.generated.h"

/**
 * 
 */
 UENUM(BlueprintType)
 enum class EItemName : uint8
 {
	EIN_Wood	UMETA(DisplayName = "Wood"),
	ETN_Smalltwig UMETA(DisplayName = "Smalltwig"),
	ETN_Stone UMETA(DisplayName = "Stone"),
	ETN_BigStone UMETA(DisplayName = "BigStone"),
	ETN_MetalOre UMETA(DisplayName = "MetalOre"),
	ETN_Fabric UMETA(DisplayName = "Fabric"),
	ETN_Glass UMETA(DisplayName = "Glass"),
	ETN_Plastic UMETA(DisplayName = "Plastic"),
	ETN_Bone UMETA(DisplayName = "Bone"),
	ETN_SoulStone UMETA(DisplayName = "SoulStone"),
	ETN_SoulFragment UMETA(DisplayName = "SoulFragment"),
	ETN_WoodFoundation UMETA(DisplayName = "WoodFoundation"), // 나무 의자 같은거
	ETN_WoodWall UMETA(DisplayName = "WoodWall"),
	ETN_WoodenRoof UMETA(DisplayName = "WoodenRoof"),
	ETN_Woodfloor UMETA(DisplayName = "Woodfloor"),

	ETN_MAX	UMETA(DisplayName = "MAX")

 };


 USTRUCT(BlueprintType)
 struct FRecipe
 {
	GENERATED_BODY() 
	
	// the Recipe name
	UPROPERTY(EditAnywhere, Category = "Recipe")
	EItemName ItemType;

	// the Recipe amount
	UPROPERTY(EditAnywhere, Category = "Recipe")
	uint8 Amount;
 };


 USTRUCT(BlueprintType)
struct FCraftItemInfo
{

	GENERATED_BODY()

	// the Recipe name in text
	UPROPERTY(EditAnywhere, Category = "Recipe")
	FText ItemName;

	// the item icon
	UPROPERTY(EditAnywhere, Category = "Recipe")
	FSlateBrush ItemIcon;
};

//TMap<EItemName, FCraftItemInfo> CraftItemData;

USTRUCT(BlueprintType)
 struct FCraftItem
 {

	GENERATED_BODY()
	
	// the craftitem name.
	UPROPERTY(EditAnywhere, Category = "Recipe")
	EItemName CraftedItem;

	// the craftitem amount.
	UPROPERTY(EditAnywhere, Category = "Recipe")
	uint8 CraftedItemAmount;
 
	// the Recipe needed to craft the item.
	UPROPERTY(EditAnywhere, Category = "Recipe")
	TArray<FRecipe> CraftRecipes;
 };




 



UCLASS()
class PIXELCODE_API UCreateItemData : public UObject
{
	GENERATED_BODY()
	
};
