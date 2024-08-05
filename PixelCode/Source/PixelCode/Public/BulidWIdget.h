// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTypes.h"
#include "CreateItemData.h"
#include "BulidWIdget.generated.h"



class AItemStorage;
class UScrollBox;
class APixelCodeCharacter;
class UTextBlock;
class UImage;
class UHorizontalBox;
class UButton;
class USoundBase;
class UBuildItemSlot;
/**
 * 
 */
UCLASS()
class PIXELCODE_API UBulidWIdget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

public:

	void SetBuildInfo(uint16 Index, uint16 ButtonIndex);

protected:
	/*UPROPERTY(meta = (BindWidget))
	UTextBlock* T_MainText;*/

	UPROPERTY(meta = (BindWidget))
	UImage* I_MainImage;


	UPROPERTY(EditAnywhere, Category = "Build")
	APixelCodeCharacter* Char;

	UPROPERTY(VisibleAnywhere, Category = "Build")
	TArray<TWeakObjectPtr<UBuildItemSlot>> BuildItems;

	// 아이템 인덱스 선택
	UPROPERTY(VisibleAnywhere, Category = "Build")
	uint8 SelectedBuidIndex;

	UPROPERTY(VisibleAnywhere, Category = "Build")
	uint8 SelectedBuilditemIndex;

	UPROPERTY(EditAnywhere, Category = "Build")
	AItemStorage* ItemStorage;

	UPROPERTY(EditAnywhere, Category = "Build")
	APixelCodeCharacter* Charar;


	UPROPERTY(VisibleAnywhere, Category = "Build")
	TArray<TWeakObjectPtr<UBuildItemSlot>> BuildItemSlot;


	FText GetBuildNameFromType(EItemName Name);

	FSlateBrush GetBuildIconFromType(EItemName Name);

	// All item that can be crafted
	UPROPERTY(EditAnywhere, Category = "CRAFT")
	TArray<FCraftItem> BuildeCrafts;




};
