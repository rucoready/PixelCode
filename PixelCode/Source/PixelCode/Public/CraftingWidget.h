// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateItemData.h"
#include "Player/inventory/InventoryComponent.h"
#include "CraftingWidget.generated.h"

class AItemStorage;
class UScrollBox;
class APixelCodeCharacter;
class UCraftItemWidget;
class UTextBlock;
class UImage;
class UHorizontalBox;;
class UCraftingSlotWidget;
class UButton;
class USoundBase;


/**
 * 
 */
UCLASS()
class PIXELCODE_API UCraftingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	void SetCraftingInfo(uint16 Index, uint16 ButtonIndex);

	// 크래프트 슬롯 초기화
	void InitializeCraftSlot();

	// 
	void RefreshCraftItem();

	UFUNCTION()
	void OnCraftClicked();

	// 플레이어 커서 버튼이 있기
	UFUNCTION()
	void OnCraftHoverd();

	UFUNCTION()
	void OnCraftUnHoverd();

	UFUNCTION()
	void OnCraftbulkPressed();

	// 크래프팅 새로고침
	void RefreshCraftingScreen();

protected:
	UPROPERTY(EditAnywhere, Category = "CRAFT")
	TSubclassOf<UCraftItemWidget>CraftItemTemplate;

	UPROPERTY(EditAnywhere, Category = "CRAFT")
	TSubclassOf<UCraftingSlotWidget>CraftingSlotTemplate;

	UPROPERTY(EditAnywhere, Category = "CRAFT")
	TSubclassOf<UUserWidget>CustomTooltipTemplate;

	// 아이템 인덱스 선택
	UPROPERTY(VisibleAnywhere, Category = "CRAFT")
	uint8 SelectedIndex;

	UPROPERTY(VisibleAnywhere, Category = "CRAFT")
	uint8 SelectedItemIndex;
	
	

	
	UPROPERTY(VisibleAnywhere, Category = "CRAFT")
	bool bCraftable;

	// 벌크 크래프트 유무
	UPROPERTY(VisibleAnywhere, Category = "CRAFT")
	bool bBulkCraft;

	bool isCraftable();
	
	UPROPERTY(VisibleAnywhere, Category = "CRAFT")
	TArray<TWeakObjectPtr<UCraftItemWidget>> CraftItems;

	UPROPERTY(EditAnywhere, Category = "CRAFT")
	AItemStorage* ItemStorage;

	UPROPERTY(EditAnywhere, Category = "CRAFT")
	APixelCodeCharacter* Char;

	// All item that can be crafted
	UPROPERTY(EditAnywhere, Category = "CRAFT")
	TArray<FCraftItem> Crafts;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* CraftList;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_ItemName;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Icon;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* item_Recipes;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Craft;

	// 크래프팅 성공 사운드
	UPROPERTY(EditDefaultsOnly)
	USoundBase* CraftSuccessSound;

	UPROPERTY(EditAnywhere, Category = "KYH")
	class APickup* PickupItems;

	UPROPERTY()
	UInventoryComponent* OwningInventory; 

	// 아이템 만드는 함수
	void MakeCraftItem(uint16 Index, uint16 ButtonIndex, const FText& ItemName);

	


	// 레시피 슬롯 베이스
	void CreateCraftRecipeSlot(const FRecipe& Recipe);
	
	
	FText GetItemNameFromType(EItemName Name);

	FSlateBrush GetItemIconFromType(EItemName Name);
};
