// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h" // UImage 포함
#include "DamageWidget.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UDamageWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	virtual void NativeConstruct() override;

	// Button and text block widgets
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* damageText;	

	//UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	//class UImage* damageImg;

	void PlayDamageAnimation();

	void HideDamage();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	class USoundCue* critsDamageSound;

	void SetDamage();

	void DamageSystem();

	void PlayDamageAnimation01(int damageValue);

	void PlayDamageAnimation02(int damageValue2);

	void PlayDamageAnimation03(int damageValue3);

	void PlayDamageAnimation04(int damageValue4);

	void PlayDamageAnimation05(int damageValue5);

	int32 value;

	int32 damageAmount;

	UPROPERTY(Editanywhere, Category="MySettings", meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* DamageAnimation;

	UPROPERTY(Editanywhere, Category="MySettings", meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* DamageAnimation2;

	UPROPERTY(Editanywhere, Category="MySettings", meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* DamageAnimation3;

	UPROPERTY(Editanywhere, Category="MySettings", meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* DamageAnimation4;

	UPROPERTY(Editanywhere, Category="MySettings", meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* DamageAnimation5;

	FTimerHandle timerhandle_DamageHide;
};
