// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyMapLodingWidget.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UMyMapLodingWidget : public UUserWidget
{
	GENERATED_BODY()
	

	

	virtual void NativeConstruct() override;

	// ÇÁ·Î±×·¡½º¹Ù À§Á¬
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UProgressBar* loadingMyMap;
	

	FTimerHandle timerHandle_TimerMymapProgressLoading;
	float progressed;
	float elapsedTimes;
	const float totalDurations = 4.0f; // ÃÑ 7ÃÊ

	void UpdateMyMapProgressBar();	

};
