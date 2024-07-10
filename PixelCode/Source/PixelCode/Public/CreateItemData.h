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
	ETN_NONE = 0			UMETA(DisplayName = "NOne"),
	EIN_Wood = 2			UMETA(DisplayName = "Wood"),
	ETN_Smalltwig = 3		UMETA(DisplayName = "Smalltwig"),
	ETN_Stone = 4			UMETA(DisplayName = "Stone"),
	ETN_BigStone = 5		UMETA(DisplayName = "BigStone"),
	ETN_MetalOre = 6		UMETA(DisplayName = "MetalOre"),
	ETN_Fabric = 7			UMETA(DisplayName = "Fabric"),
	ETN_Glass = 8			UMETA(DisplayName = "Glass"),
	ETN_Plastic = 9			UMETA(DisplayName = "Plastic"),
	ETN_Bone = 10			UMETA(DisplayName = "Bone"),
	ETN_SoulStone = 11		UMETA(DisplayName = "SoulStone"),
	ETN_SoulFragment = 12	UMETA(DisplayName = "SoulFragment"),
	ETN_WoodFoundation = 40 UMETA(DisplayName = "WoodFoundation"), // 나무 의자 같은거
	ETN_WoodWall = 41		UMETA(DisplayName = "WoodWall"),
	ETN_WoodenRoof = 42		UMETA(DisplayName = "WoodenRoof"),
	ETN_Woodfloor = 43		UMETA(DisplayName = "Woodfloor"),

	ETN_MAX					UMETA(DisplayName = "MAX")

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
