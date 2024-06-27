// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingWidget.generated.h"

class UScrollBox;
class UCraftItemWidget;

/**
 * 
 */
UCLASS()
class PIXELCODE_API UCraftingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, Category = "CRAFT")
	TSubclassOf<UCraftItemWidget>CraftItemClass;
	
	UPROPERTY(EditAnywhere, Category = "CRAFT")
	TArray<TWeakObjectPtr<UCraftItemWidget>> CraftItems;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* CraftList;

	// 아이템 만드는 함수
	void MakeCraftItem(uint8 Index,const FText& ItemName);
};
