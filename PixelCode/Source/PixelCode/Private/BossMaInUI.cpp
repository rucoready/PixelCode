// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMaInUI.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "Components/ProgressBar.h"
#include "Boss/BossApernia.h"

void UBossMaInUI::NativeConstruct()
{
	

	
	
}

void UBossMaInUI::UpdateHPBar(float newHP)
{
	if (ProgressBar_hp)
	{
		// 새로운 HP 값을 100으로 나누어 0.0에서 1.0 사이의 비율(Percentage)로 변환
		float Percentage = static_cast<float>(newHP) / 100000.0f;
		// 변환된 비율을 사용하여 HP 프로그레스 바의 진행 상태를 업데이트
		ProgressBar_hp->SetPercent(Percentage);
	}
}
