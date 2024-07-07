// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemStorage.h"

// Sets default values
AItemStorage::AItemStorage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	ItemDataTable = ConstructorHelpers::FObjectFinder<UDataTable>(TEXT("/Script/Engine.DataTable'/Game/YoHan/DataTable/itembase_-_itembase.itembase_-_itembase'")).Object;

}

TSubclassOf<AActor> AItemStorage::GetTemplateOfItem(uint8 Id)
{
	return ItemTemplates[Id];
	//UPROPERTY(EditDefaultsOnly, Category = "Pickup | Item Initialization")
	//FName DesiredItemID;
	
}

TSubclassOf<AActor> AItemStorage::GetTemplateOfItem(EItemName ItemName)
{
	uint8 ItemId = (uint8) ItemName;
	UE_LOG(LogTemp, Warning, TEXT("%d"), ItemId);

	return GetTemplateOfItem(ItemId);
}

TArray<FCraftItem> AItemStorage::GetAllCrafting()
{
	UE_LOG(LogTemp, Warning, TEXT("777777777777777777777"))
	return Crafting;
}

FCraftItemInfo AItemStorage::GetCraftItemInfoBasedOn(EItemName Name)
{
	return CraftItemData[Name];
}

// Called when the game starts or when spawned
void AItemStorage::BeginPlay()
{
	Super::BeginPlay();
	
}

