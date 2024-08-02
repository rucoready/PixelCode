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

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* exitButton;

	UFUNCTION()
	void OnMyclickButtonEnter();

	UFUNCTION()
	void OnMyclickExit();

	void ServerTravel();

	class APCodePlayerController* PCodePlayerController;

	FTimerHandle timerhandle_ServerTravel;
	
};
