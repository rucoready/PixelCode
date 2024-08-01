// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMapWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MyGameModeBase.h"
#include "Net/UnrealNetwork.h"
#include "PortalCollision.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PixelCodeCharacter.h"
#include "PCodePlayerController.h"
#include "LoadingWidget1.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"

void UMyMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MapTravelButton)
	{
		MapTravelButton->OnClicked.AddDynamic(this, &UMyMapWidget::OnMyclickMyMap);
	}
	if (Button_MyMapBack)
	{
		Button_MyMapBack->OnClicked.AddDynamic(this, &UMyMapWidget::OnMyMapExit);
	}
}

void UMyMapWidget::OnMyclickMyMap()
{
	MyServerTravel();
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		// PCodePlayerController 타입으로 캐스팅
		PCodePlayerController = Cast<APCodePlayerController>(It->Get());
		if (PCodePlayerController)
		{
			PCodePlayerController->ServerRPC_CreateWidgetMyMap();
			//UE_LOG(LogTemp, Warning, TEXT("I55"));
		}
	}
}

void UMyMapWidget::MyServerTravel()
{
	GetWorld()->ServerTravel(TEXT("/Game/KSH/Maps/Map?Listen"));
}

void UMyMapWidget::OnMyMapExit()
{

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		// PCodePlayerController 타입으로 캐스팅
		PCodePlayerController = Cast<APCodePlayerController>(It->Get());
		if (PCodePlayerController)
		{
			PCodePlayerController->ServerRPC_HidMyMap();
		}
	}
}
