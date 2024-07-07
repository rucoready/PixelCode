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

	// id를 기반으로 클래스 가져오기
	TSubclassOf<AActor> GetTemplateOfItem(uint8 Id);


	TSubclassOf<AActor> GetTemplateOfItem(EItemName ItemName);
	

	// 테스트 =============================================================
	UPROPERTY(EditInstanceOnly, Category = "Item")
	UDataTable* ItemDataTable;

	// 모든 제작
	TArray<FCraftItem> GetAllCrafting();

	// EItemName 기반 크래프트 품목 정보
	FCraftItemInfo GetCraftItemInfoBasedOn(EItemName Name);

	//  템플릿
	UPROPERTY(EditAnywhere, Category = "Item")
	TMap<uint8, TSubclassOf<AActor>> ItemTemplates;
	

	//  데이터들 , EItemName 기반 텍스트 및 아이콘
	UPROPERTY(EditAnywhere, Category = "Item")
	TMap<EItemName, FCraftItemInfo> CraftItemData;

	UPROPERTY(EditAnywhere, Category = "Item")
	TArray<FCraftItem> Crafting;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	


	
};
