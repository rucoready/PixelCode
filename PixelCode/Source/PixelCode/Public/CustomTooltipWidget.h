// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomTooltipWidget.generated.h"


/**
 * 
 */
UCLASS()
class PIXELCODE_API UCustomTooltipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 아이템 이름 설정
	void SetItemName(const FText& Name);
	

protected:

	virtual void NativeConstruct() override;


protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* T_ItemName;
	
};
