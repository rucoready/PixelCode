// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemStorage.h"

// Sets default values
AItemStorage::AItemStorage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

TSubclassOf<AActor> AItemStorage::GetTemplateOfItem(uint8 Id)
{
	return ItemTemplates[Id];
	
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

