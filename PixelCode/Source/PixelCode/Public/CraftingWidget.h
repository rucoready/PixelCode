// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateItemData.h"
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


/**
 * 
 */
UCLASS()
class PIXELCODE_API UCraftingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	void SetCraftingInfo(uint8 Index);

	// ũ����Ʈ ���� �ʱ�ȭ
	void InitializeCraftSlot();

	UFUNCTION()
	void OnCraftClicked();

protected:
	UPROPERTY(EditAnywhere, Category = "CRAFT")
	TSubclassOf<UCraftItemWidget>CraftItemTemplate;

	UPROPERTY(EditAnywhere, Category = "CRAFT")
	TSubclassOf<UCraftingSlotWidget>CraftingSlotTemplate;

	UPROPERTY(EditAnywhere, Category = "CRAFT")
	TSubclassOf<UUserWidget>CustomTooltipTemplate;

	// ������ �ε��� ����
	UPROPERTY(VisibleAnywhere, Category = "CRAFT")
	uint8 SelectedIndex;
	

	
	UPROPERTY(VisibleAnywhere, Category = "CRAFT")
	bool bCraftable;

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

	// ������ ����� �Լ�
	void MakeCraftItem(uint16 Index,const FText& ItemName);

	


	// ������ ���� ���̽�
	void CreateCraftSlot(const FRecipe& Recipe);
	
	
	FText GetItemNameFromType(EItemName Name);

	FSlateBrush GetItemIconFromType(EItemName Name);
};
