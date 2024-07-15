// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossMaInUI.generated.h"


/**
 * 
 */
UCLASS()
class PIXELCODE_API UBossMaInUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UProgressBar* ProgressBar_hp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MySettings")
	float HP = 0.4f;

	void UpdateHPBar(float newHP);

	
};
