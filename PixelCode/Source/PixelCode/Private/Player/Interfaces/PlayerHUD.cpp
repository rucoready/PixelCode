// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Interfaces/PlayerHUD.h"
#include "Player/Interfaces/MainMenuWidget.h"
#include "Player/Interfaces/InteractionWidget.h"
#include "Player/PlayerStatWidget.h"
#include "CraftingWidget.h"
#include "BulidWIdget.h"
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
		MainManuWidget->SetVisibility(ESlateVisibility::Collapsed); // 축소설정,Hide와 비슷
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	/*if (Crafting)
	{
		Crafting = CreateWidget<UCraftingWidget>(GetWorld(), CraftingClass);
		Crafting->AddToViewport(2);
		Crafting->SetVisibility(ESlateVisibility::Collapsed);
	}*/
	
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

		const FInputModeGameOnly InputMode; // 게임화면만 클릭하도록 설정
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayMenu();

		const FInputModeGameAndUI InputMode; // UI만 클릭하도록 설정
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

//=================== 요한 ===============================================================================================

void APlayerHUD::ShowOrHideCrafting()
{
	if (Crafting)
	{

		creatwidgets();
		const FInputModeGameOnly InputMode;
		Crafting->SetVisibility(ESlateVisibility::Visible);
		Crafting->RefreshCraftingScreen();
		bIsCreatVisible = true;
		GetOwningPlayerController()->SetShowMouseCursor(true);
		GetOwningPlayerController()->SetInputMode(InputMode);
	}
}

void APlayerHUD::HideCrafting()
{
	if (Crafting)
	{
		Crafting->SetVisibility(ESlateVisibility::Collapsed); 
		bIsCreatVisible = false;
		const FInputModeGameOnly InputMode; // 게임화면만 클릭하도록 설정
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
}

void APlayerHUD::ToggleCreate()
{
	if (bIsCreatVisible)
	{
		HideCrafting();
		//creatwidgets();

		const FInputModeGameOnly InputMode; // 게임화면만 클릭하도록 설정
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		ShowOrHideCrafting();
		

		const FInputModeGameAndUI InputMode; // UI만 클릭하도록 설정
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}

}

void APlayerHUD::creatwidgets()
{
	if (Crafting)
	{
		Crafting = CreateWidget<UCraftingWidget>(GetWorld(), CraftingClass);
		Crafting->AddToViewport(2);
		Crafting->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	

}

void APlayerHUD::ShowBuilding()
{
	if (Building)
	{

		Buildwidgets();
		const FInputModeGameOnly InputMode;
		Building->SetVisibility(ESlateVisibility::Visible);
		//Building->RefreshCraftingScreen();
		bIsbuildVisible = true;
		GetOwningPlayerController()->SetShowMouseCursor(true);
		GetOwningPlayerController()->SetInputMode(InputMode);
	}
}

void APlayerHUD::HideBuildinging()
{
	if (Building)
	{
		Building->SetVisibility(ESlateVisibility::Collapsed);
		bIsbuildVisible = false;
		const FInputModeGameOnly InputMode; // 게임화면만 클릭하도록 설정
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
}

void APlayerHUD::ToggleBuilding()
{
	if (bIsbuildVisible)
	{
		HideBuildinging();

		const FInputModeGameOnly InputMode; // 게임화면만 클릭하도록 설정
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		ShowBuilding();
		//creatwidgets();

		const FInputModeGameAndUI InputMode; // UI만 클릭하도록 설정
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void APlayerHUD::Buildwidgets()
{
	if (Building)
	{
		Building = CreateWidget<UBulidWIdget>(GetWorld(), BuildingClass);
		Building->AddToViewport();
		Building->SetVisibility(ESlateVisibility::Collapsed);
	}
}
