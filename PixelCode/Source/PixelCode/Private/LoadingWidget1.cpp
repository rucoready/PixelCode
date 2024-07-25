// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingWidget1.h"
#include "Components/ProgressBar.h"

void ULoadingWidget1::NativeConstruct()
{
	Super::NativeConstruct();

	// 초기화
	progress = 0.0f;
	elapsedTime = 0.0f;
	if (loadingBar)
	{
		loadingBar->SetPercent(progress);
	};

	// 타이머 시작 (0.01초마다 호출하여 부드러운 업데이트)
	GetWorld()->GetTimerManager().SetTimer(timerHandle_TimerhandleProgressLoading, this, &ULoadingWidget1::UpdateProgressBar, 0.01f, true);
}

void ULoadingWidget1::UpdateProgressBar()
{
	if (loadingBar)
	{
		// 남은 시간에 비례하여 증가량 계산
		float remainingTime = totalDuration - elapsedTime;
		float randomFactor = FMath::FRandRange(0.9f, 1.1f); // 랜덤 요소 추가
		float increment = (randomFactor * 0.01f) / totalDuration;

		progress += increment;
		elapsedTime += 0.01f;

		// 퍼센트가 1을 넘지 않도록 하고 총 시간이 7초를 넘으면 타이머를 정지함
		if (elapsedTime >= totalDuration)
		{
			progress = 1.0f;
			GetWorld()->GetTimerManager().ClearTimer(timerHandle_TimerhandleProgressLoading); // 타이머 정지
		}

		loadingBar->SetPercent(progress);
	}
}