// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildItemSlot.generated.h"


class UImage;
class UButton;
class UTextBlock;
class UBulidWIdget;

/**
 * 
 */
UCLASS()
class PIXELCODE_API UBuildItemSlot : public UUserWidget
{
	GENERATED_BODY()
	

public:
	// 슬롯 배경 색상 변경
	//void SetBackgroundColorBase(bool bAvailable);

	//슬롯 데이터 설정

	void SetData(uint16 Index, uint16 BuildIndexs, const FText& BItemName,const FSlateBrush& Icon, TWeakObjectPtr<UBulidWIdget> Build);

	UFUNCTION()
	void OnBuildItemClicked();

	UFUNCTION()
	void ActivateBuildButton(bool bBActive);

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_BuildItemName;

	UPROPERTY(meta = (BindWidget))
	UImage* I_BuildImage;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_BuildButton;

	UPROPERTY(meta = (BindWidget))
	uint16 BuildIndex;

	UPROPERTY(meta = (BindWidget))
	uint16 ButtonBuildIndex;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TWeakObjectPtr<UBulidWIdget> BuildWidget;



	// 버튼 컬러
	UPROPERTY(EditAnywhere, Category = "Widget")
	FLinearColor SelectedBuildColor;

	UPROPERTY(EditAnywhere, Category = "Widget")
	FLinearColor DefalutBuildColor;
};
