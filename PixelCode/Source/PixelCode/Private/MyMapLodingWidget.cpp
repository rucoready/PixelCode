// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMapLodingWidget.h"
#include "Components/ProgressBar.h"

void UMyMapLodingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 초기화
	progressed = 0.0f;
	elapsedTimes = 0.0f;
	if (loadingMyMap)
	{
		loadingMyMap->SetPercent(progressed);
	};

	// 타이머 시작 (0.01초마다 호출하여 부드러운 업데이트)
	GetWorld()->GetTimerManager().SetTimer(timerHandle_TimerMymapProgressLoading, this, &UMyMapLodingWidget::UpdateMyMapProgressBar, 0.01f, true);
}

void UMyMapLodingWidget::UpdateMyMapProgressBar()
{
	if (loadingMyMap)
	{
		// 남은 시간에 비례하여 증가량 계산
		float remainingTime = totalDurations - elapsedTimes;
		float randomFactor = FMath::FRandRange(0.9f, 1.1f); // 랜덤 요소 추가
		float increment = (randomFactor * 0.01f) / totalDurations;

		progressed += increment;
		elapsedTimes += 0.01f;

		// 퍼센트가 1을 넘지 않도록 하고 총 시간이 7초를 넘으면 타이머를 정지함
		if (elapsedTimes >= totalDurations)
		{
			progressed = 1.0f;
			GetWorld()->GetTimerManager().ClearTimer(timerHandle_TimerMymapProgressLoading); // 타이머 정지
		}

		loadingMyMap->SetPercent(progressed);
	}

}
