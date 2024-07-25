// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingWidget1.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API ULoadingWidget1 : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// 프로그래스바 위젯
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UProgressBar* loadingBar;

private:
	FTimerHandle timerHandle_TimerhandleProgressLoading;
	float progress;
	float elapsedTime;
	const float totalDuration = 7.0f; // 총 7초

	void UpdateProgressBar();
};
