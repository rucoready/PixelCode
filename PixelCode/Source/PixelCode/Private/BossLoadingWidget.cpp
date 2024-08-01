// Fill out your copyright notice in the Description page of Project Settings.


#include "BossLoadingWidget.h"
#include "Components/ProgressBar.h"

void UBossLoadingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	progress = 0.0f;
	elapsedTime = 0.0f;
	if (loadingBar)
	{
		loadingBar->SetPercent(progress);
	};

	GetWorld()->GetTimerManager().SetTimer(timerHandle_TimerhandleProgressLoading, this, &UBossLoadingWidget::UpdateProgressBar, 0.01f, true);
}

void UBossLoadingWidget::UpdateProgressBar()
{
	if (loadingBar)
	{
		float remainingTime = totalDuration - elapsedTime;
		float randomFactor = FMath::FRandRange(0.9f, 1.1f); 
		float increment = (randomFactor * 0.01f) / totalDuration;

		progress += increment;
		elapsedTime += 0.01f;

		if (elapsedTime >= totalDuration)
		{
			progress = 1.0f;
			GetWorld()->GetTimerManager().ClearTimer(timerHandle_TimerhandleProgressLoading); // 타이머 정지
		}

		loadingBar->SetPercent(progress);
	}
}
