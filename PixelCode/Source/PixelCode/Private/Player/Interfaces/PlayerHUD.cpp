// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Interfaces/PlayerHUD.h"
#include "Player/Interfaces/MainMenuWidget.h"
#include "Player/Interfaces/InteractionWidget.h"
//#include "LootPanel.h"

APlayerHUD::APlayerHUD()
{

}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetClass)
	{
		MainManuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
		MainManuWidget->AddToViewport(5);
		MainManuWidget->SetVisibility(ESlateVisibility::Collapsed); // ��Ҽ���,Hide�� ���
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APlayerHUD::DisplayMenu()
{
	if (MainManuWidget)
	{
		bIsMenuVisible = true;
		MainManuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerHUD::HideMenu()
{
	if (MainManuWidget)
	{
		bIsMenuVisible = false;
		MainManuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APlayerHUD::ToggleMenu()
{
	if (bIsMenuVisible)
	{
		HideMenu();

		const FInputModeGameOnly InputMode; // ����ȭ�鸸 Ŭ���ϵ��� ����
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayMenu();

		const FInputModeGameAndUI InputMode; // UI�� Ŭ���ϵ��� ����
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}


}

void APlayerHUD::ShowInteractionWidget()const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerHUD::HideInteractionWidget()const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APlayerHUD::UpdateInteractionWidget(const FInteractableData& InteractableData)const
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}

}