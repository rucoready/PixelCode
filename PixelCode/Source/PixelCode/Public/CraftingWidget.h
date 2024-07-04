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

protected:
	UPROPERTY(EditAnywhere, Category = "CRAFT")
	TSubclassOf<UCraftItemWidget>CraftItemTemplate;

	// 아이템 인덱스 선택
	UPROPERTY(VisibleAnywhere, Category = "CRAFT")
	uint8 SelectedIndex;
	
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


	// 아이템 만드는 함수
	void MakeCraftItem(uint16 Index,const FText& ItemName);
};
