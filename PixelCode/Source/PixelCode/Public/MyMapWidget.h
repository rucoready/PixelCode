// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyMapWidget.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UMyMapWidget : public UUserWidget
{
	GENERATED_BODY()


	virtual void NativeConstruct() override;

	// Button and text block widgets
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* MapTravelButton;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* Button_MyMapBack;

	UFUNCTION()
	void OnMyclickMyMap();

	void MyServerTravel();

	UFUNCTION()
	void OnMyMapExit();

	class APCodePlayerController* PCodePlayerController;
};
