// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossEnterWidget.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UBossEnterWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	// Button and text block widgets
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* enterButton;

	UFUNCTION()
	void OnMyclickButtonEnter();

	void ServerTravel();

	class APCodePlayerController* PCodePlayerController;
	
};
