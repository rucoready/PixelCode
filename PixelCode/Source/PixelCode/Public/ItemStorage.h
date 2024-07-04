// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreateItemData.h"
#include "ItemStorage.generated.h"

UCLASS()
class PIXELCODE_API AItemStorage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemStorage();

	// get the item class based on id
	TSubclassOf<AActor> GetTemplateOfItem(uint8 Id);

	// get all crafting data
	TArray<FCraftItem> GetAllCrafting();

	// get craft item info(name in text n icon) based on EItemName value
	FCraftItemInfo GetCraftItemInfoBasedOn(EItemName Name);

	// Template of all of the item
	UPROPERTY(EditAnywhere, Category = "Item")
	TMap<uint8, TSubclassOf<AActor>> ItemTemplates;
	 

	// the data of craft item. item Name In Text and Icon based On EItemName
	UPROPERTY(EditAnywhere, Category = "Item")
	TMap<EItemName, FCraftItemInfo> CraftItemData;

	UPROPERTY(EditAnywhere, Category = "Item")
	TArray<FCraftItem> Crafting;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	


	
};
