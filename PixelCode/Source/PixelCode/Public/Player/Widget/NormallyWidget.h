// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NormallyWidget.generated.h"

class UProgressBar;
class UStateComponent;
class ApixelPlayerState;
class APixelCodeCharacter;
class APlayerController;

/**
 * 
 */
UCLASS()
class PIXELCODE_API UNormallyWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:

	//virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;


	UPROPERTY(EditDefaultsOnly, Category = "Stat", meta = (BindWidget))
	UProgressBar* PB_HP;

	UPROPERTY(EditDefaultsOnly, Category = "Stat", meta = (BindWidget))
	UProgressBar* PB_MP;

	UPROPERTY(EditDefaultsOnly, Category = "Stat", meta = (BindWidget))
	UProgressBar* PB_Exp;

	UPROPERTY(EditDefaultsOnly, Category = "Stat")
	UStateComponent* statComp;

	UPROPERTY(EditDefaultsOnly, Category = "Exp")
	ApixelPlayerState* playerState;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	APixelCodeCharacter* Player;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	APlayerController* PlayerController;

	void firstUpdate();
	

	void currentStatUpdate();
	void currentExpUpdate();
	

};
