// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "Engine/World.h"  // GetWorld()를 사용하기 위해 필요
#include "Animation/WidgetAnimation.h"

void UDamageWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HideDamage();
	

}

void UDamageWidget::PlayDamageAnimation()
{

	
	
}

void UDamageWidget::HideDamage()
{
	if (damageText)
	{
		damageText->SetVisibility(ESlateVisibility::Collapsed);
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
