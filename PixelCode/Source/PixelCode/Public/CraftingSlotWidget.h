// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingSlotWidget.generated.h"

class UImage;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PIXELCODE_API UCraftingSlotWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	// 슬롯 배경 색상 변경
	void SetBackgroundColorBase(bool bAvailable);

	// 슬롯 데이터 설정
	void SetData(const FText& Amount, const FSlateBrush& Icon);
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* I_BorderColor;

	UPROPERTY(meta = (BindWidget))
	UImage* I_ItemIcon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_ItemAmount;

	UPROPERTY(EditAnywhere, Category = "Colors")
	FLinearColor AvailableColor;

	UPROPERTY(EditAnywhere, Category = "Colors")
	FLinearColor NotAvailableColor;


};
