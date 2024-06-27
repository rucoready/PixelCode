// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatWidget.generated.h"


class UTextBlock;
class UStateComponent;

/**
 * 
 */
UCLASS()
class PIXELCODE_API UPlayerStatWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	
	//이름, 레벨 hp mp atk def critical str dex int luck con
	UPROPERTY(VisibleAnywhere,Category = "Stat", meta = (BindWidget))
	UTextBlock* TB_NAME;

	UPROPERTY(VisibleAnywhere,Category = "Stat", meta = (BindWidget))
	UTextBlock* TB_LEVEL;

	UPROPERTY(VisibleAnywhere,Category = "Stat", meta = (BindWidget))
	UTextBlock* TB_HP;

	UPROPERTY(VisibleAnywhere,Category = "Stat", meta = (BindWidget))
	UTextBlock* TB_MP;

	UPROPERTY(VisibleAnywhere,Category = "Stat", meta = (BindWidget))
	UTextBlock* TB_ATK;

	UPROPERTY(VisibleAnywhere,Category = "Stat", meta = (BindWidget))
	UTextBlock* TB_DEF;

	UPROPERTY(VisibleAnywhere,Category = "Stat", meta = (BindWidget))
	UTextBlock* TB_Critical;

	UPROPERTY(VisibleAnywhere,Category = "Stat", meta = (BindWidget))
	UTextBlock* TB_STR;

	UPROPERTY(VisibleAnywhere,Category = "Stat", meta = (BindWidget))
	UTextBlock* TB_DEX;

	UPROPERTY(VisibleAnywhere,Category = "Stat", meta = (BindWidget))
	UTextBlock* TB_INT;

	UPROPERTY(VisibleAnywhere,Category = "Stat", meta = (BindWidget))
	UTextBlock* TB_LUCK;

	UPROPERTY(VisibleAnywhere,Category = "Stat", meta = (BindWidget))
	UTextBlock* TB_CON;


	UStateComponent* statComp;

	void UpdateStat();

	void DisplayStat();
	void HideStat();

	//virtual void NativeConstruct() override;
	
	

};
