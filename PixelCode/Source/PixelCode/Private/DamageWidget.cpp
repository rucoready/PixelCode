// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "Engine/World.h"  // GetWorld()를 사용하기 위해 필요
#include "Sound/SoundCue.h" // SoundCue 포함
#include "Kismet/GameplayStatics.h" // UGameplayStatics 포함
#include "Animation/WidgetAnimation.h"

void UDamageWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HideDamage();
	

}

void UDamageWidget::PlayDamageAnimation()
{
// 	value = FMath::RandRange(1, 5);
// 
// 
// 	if (value == 1)
// 	{
// 		PlayAnimation(DamageAnimation);
// 		/*damageAmount = FMath::RandRange(800, 900);*/
// 	}
// 	else if (value == 2)
// 	{
// 		PlayAnimation(DamageAnimation2);
// 		/*damageAmount = FMath::RandRange(2000, 2100);*/
// 	}
// 	else if (value == 3)
// 	{
// 		PlayAnimation(DamageAnimation3);
// 		/*damageAmount = FMath::RandRange(700, 800);*/
// 	}
// 	else if (value == 4)
// 	{
// 		PlayAnimation(DamageAnimation4);
// 		/*damageAmount = FMath::RandRange(500, 1000);*/
// 	}
// 	else
// 	{
// 		PlayAnimation(DamageAnimation5);
// 		/*damageAmount = FMath::RandRange(1000,1200);*/
// 	}
// 
// 	/*damageText->SetText(FText::AsNumber(damageAmount));*/

	
	
}

void UDamageWidget::HideDamage()
{
	if (damageText)
	{
		damageText->SetVisibility(ESlateVisibility::Collapsed);
		//damageImg->SetVisibility(ESlateVisibility::Collapsed);
	}
}



void UDamageWidget::SetDamage()
{
	if (damageText)
	{
		damageText->SetVisibility(ESlateVisibility::Visible);
		
	}
}

void UDamageWidget::DamageSystem()
{
	PlayDamageAnimation();
	
}

void UDamageWidget::PlayDamageAnimation01(int damageValue)
{ 
	PlayAnimation(DamageAnimation);
	damageText->SetText(FText::AsNumber(damageValue));
	
}

void UDamageWidget::PlayDamageAnimation02(int damageValue2)
{
	PlayAnimation(DamageAnimation2);
	damageText->SetText(FText::AsNumber(damageValue2));
	//damageImg->SetVisibility(ESlateVisibility::Visible);
	if (critsDamageSound)
	{
		UGameplayStatics::PlaySound2D(this, critsDamageSound);
	}
	
}

void UDamageWidget::PlayDamageAnimation03(int damageValue3)
{
	PlayAnimation(DamageAnimation3);
	damageText->SetText(FText::AsNumber(damageValue3));
	
}

void UDamageWidget::PlayDamageAnimation04(int damageValue4)
{
	PlayAnimation(DamageAnimation4);
	damageText->SetText(FText::AsNumber(damageValue4));
	
}

void UDamageWidget::PlayDamageAnimation05(int damageValue5)
{
	PlayAnimation(DamageAnimation5);
	damageText->SetText(FText::AsNumber(damageValue5));
	
}
