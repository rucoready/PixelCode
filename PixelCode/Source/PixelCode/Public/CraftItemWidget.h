// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftItemWidget.generated.h"


class UButton;
class UTextBlock;
class UCraftingWidget;
/**
 * 
 */
UCLASS()
class PIXELCODE_API UCraftItemWidget : public UUserWidget
{
	GENERATED_BODY()
	



public:

	virtual void NativeConstruct() override;

	UFUNCTION() 
	void OnCraftItemClicked();

	// ������ ���� ��ȣ ����
	UFUNCTION() 
	void SetData(uint16 Index, const FText&ItemName, TWeakObjectPtr<UCraftingWidget> Craft);
	
	// ������ ��ȣ 
	UPROPERTY(meta = (BindWidget))
	uint16 ItemIndex;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_CItem;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_CraftItemName;

	// ũ������ ���� ����
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TWeakObjectPtr<UCraftingWidget> CraftingWidget;
};
