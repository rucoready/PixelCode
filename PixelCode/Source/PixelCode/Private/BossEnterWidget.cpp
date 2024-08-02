// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnterWidget.h"
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

void UBossEnterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (enterButton)
	{
		enterButton->OnClicked.AddDynamic(this, &UBossEnterWidget::OnMyclickButtonEnter);
	}
	if (exitButton)
	{
		exitButton->OnClicked.AddDynamic(this, &UBossEnterWidget::OnMyclickExit);
	}
}

void UBossEnterWidget::OnMyclickButtonEnter()
{
	//ServerTravel();
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		// PCodePlayerController 타입으로 캐스팅
		PCodePlayerController = Cast<APCodePlayerController>(It->Get());
		if (PCodePlayerController)
		{
			PCodePlayerController->ServerRPC_CreateWidgetBossLoading();
			GetWorld()->GetTimerManager().SetTimer(timerhandle_ServerTravel, this, &UBossEnterWidget::ServerTravel, 7.f, false);
		}
	}
}

void UBossEnterWidget::OnMyclickExit()
{
	UE_LOG(LogTemp, Warning, TEXT("I55"));
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		// PCodePlayerController 타입으로 캐스팅
		PCodePlayerController = Cast<APCodePlayerController>(It->Get());
		if (PCodePlayerController)
		{
			PCodePlayerController->ServerRPC_HideLastBossPortal();
		}
	}
}

void UBossEnterWidget::ServerTravel()
{
	GetWorld()->ServerTravel(TEXT("/Game/KMS_AI/BossMap/BossMap2?Listen"));
}
