// Fill out your copyright notice in the Description page of Project Settings.


#include "PCodePlayerController.h"
#include "MyGameModeBase.h"
#include "GameFramework/SpectatorPawn.h"
#include "Player/pixelPlayerState.h"
#include "Player/PlayerStatWidget.h"
#include "Player/Widget/NormallyWidget.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h>
#include "Player/PixelCodeCharacter.h"

void APCodePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GM = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
	}

	UE_LOG(LogTemp, Warning, TEXT("PC ON"));

}

void APCodePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (NormallyWidget != nullptr)
	{ 
		if (PlayerState != nullptr)
		{ 
			NormallyWidget->currentExpUpdate(PlayerState->currentEXP,PlayerState->totalEXP);
		}
	}
}

void APCodePlayerController::OpenUI(bool bOpen)
{
	if(bOpen)
	{
		SetInputMode(FInputModeGameAndUI());
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
	}

	SetShowMouseCursor(bOpen);
}


void APCodePlayerController::StartUI()
{
	auto* CharacterPlayer = Cast<APixelCodeCharacter>(GetPawn());
	PlayerState = Cast<ApixelPlayerState>(CharacterPlayer->GetPlayerState());


	if (StatWidgetClass)
	{
		statWidget = Cast<UPlayerStatWidget>(CreateWidget(GetWorld(), StatWidgetClass));
		if (statWidget != nullptr)
		{ 
			statWidget->AddToViewport(1);
			statWidget->SetVisibility(ESlateVisibility::Collapsed);
			UE_LOG(LogTemp, Warning, TEXT("NormalAuth"));
		
			statWidget = statWidget;

			statWidget->UpdateStat(CharacterPlayer->stateComp);
			if (PlayerState != nullptr)
			{
				statWidget->UpdateLevel(PlayerState->Level);
			}
		}

	}

	if (NormallyWidgetClass)
	{
		NormallyWidget = Cast<UNormallyWidget>(CreateWidget(GetWorld(), NormallyWidgetClass));
		if (NormallyWidget != nullptr)
		{ 
			NormallyWidget->AddToViewport(-1);
			NormallyWidget->SetVisibility(ESlateVisibility::Visible);
			UE_LOG(LogTemp, Warning, TEXT("NormalAuth"));

			NormallyWidget->firstUpdate(CharacterPlayer->stateComp);
			NormallyWidget->currentStatUpdate(CharacterPlayer->stateComp);
			if (PlayerState != nullptr)
			{
				NormallyWidget->currentLevelUpdate(PlayerState->Level);
			}
		}
	}
	
}

void APCodePlayerController::LevelUpUpdate()
{
	if (PlayerState != nullptr && NormallyWidget != nullptr)
	{
		NormallyWidget->currentLevelUpdate(PlayerState->Level);
	}
	if (PlayerState != nullptr && statWidget != nullptr)
	{
		statWidget->UpdateLevel(PlayerState->Level);
	}
}

//void APCodePlayerController::ServerRPC_StartUI_Implementation()
//{
//	statWidget = Cast<UPlayerStatWidget>(CreateWidget(GetWorld(), StatWidgetClass));
//	NormallyWidget = Cast<UNormallyWidget>(CreateWidget(GetWorld(), NormallyWidgetClass));
//	PlayerState = Cast<ApixelPlayerState>(GetPlayerState<ApixelPlayerState>());
//
//	if (HasAuthority())
//	{ 
//		statWidget->UpdateLevel(PlayerState->Level);
//		ClientRPC_StartUI();
//	}
//	
//}




	/*
	NormallyWidget = CreateWidget<UNormallyWidget>(GetWorld(), NormallyWidgetClass);
	NormallyWidget->AddToViewport(2);
	NormallyWidget->SetVisibility(ESlateVisibility::Visible);
	UE_LOG(LogTemp, Warning, TEXT("NormalAuth2")); */

	/*statWidget = CreateWidget<UPlayerStatWidget>(GetWorld(), StatWidgetClass);
	statWidget->AddToViewport(1);
	statWidget->SetVisibility(ESlateVisibility::Collapsed);
	UE_LOG(LogTemp, Warning, TEXT("NormalAuth"));*/


void APCodePlayerController::ClientRPC_StartUI_Implementation()
{
	statWidget = Cast<UPlayerStatWidget>(CreateWidget(GetWorld(), StatWidgetClass));
	NormallyWidget = Cast<UNormallyWidget>(CreateWidget(GetWorld(), NormallyWidgetClass));
	PlayerState = Cast<ApixelPlayerState>(GetPlayerState<ApixelPlayerState>());


	//statWidget->UpdateLevel(PlayerState->Level);
	NormallyWidget->currentExpUpdate(PlayerState->currentEXP, PlayerState->totalEXP);
}

void APCodePlayerController::StatMenu()
{
	if (bIsStatVisible)
	{
		statWidget->DisplayStat();
		bIsStatVisible = false;
		UE_LOG(LogTemp, Warning, TEXT("StatOn"));
	}

	else
	{
		statWidget->HideStat();
		bIsStatVisible = true;
		UE_LOG(LogTemp, Warning, TEXT("StatOff"));
	}
}

void APCodePlayerController::ExpUpUI()
{
	if (PlayerState)
	{
		PlayerState->SetaddUpEXP(30);

		float CurrentExp = PlayerState->GetCurrentExp();

	}


}










//void APCodePlayerController::ServerRPC_ChangeSpectator_Implementation()
//{
//	// ���� ���ΰ��� ����ϰ�ʹ�.
//	auto* oldPawn = GetPawn();
//	// ���� oldPawn�� �ִٸ�
//	if (oldPawn)
//	{
//		// �����ڸ� ���� 
//		FTransform t = oldPawn->GetActorTransform();
//		FActorSpawnParameters params;
//		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//		auto* newPawn = GetWorld()->SpawnActor<ASpectatorPawn>(GM->SpectatorClass, t, params);
//
//		// ���� �ϰ�ʹ�.
//		Possess(newPawn);
//		// oldPawn�� �ı��ϰ�ʹ�.
//		oldPawn->Destroy();
//		// 5���Ŀ� ServerRPC_RespawnPlayer_Implementation�� ȣ���ϰ�ʹ�.
//		FTimerHandle handle;
//		GetWorld()->GetTimerManager().SetTimer(handle, this, &APCodePlayerController::ServerRPC_RespawnPlayer_Implementation, 5, false);
//	}
//}

void APCodePlayerController::ServerRPC_RespawnPlayer_Implementation()
{
	// ���� ���ΰ��� ����ϰ�ʹ�.
	auto* oldPawn = GetPawn();

	// ���Ǹ� ���� �ϰ�ʹ�.
	UnPossess();

	// ����ߴ� ���ΰ��� �ı� �ϰ�ʹ�.
	if (oldPawn)
	{
		oldPawn->Destroy();
	}

	// GameMode�� ����� �Լ��� ȣ���ϰ�ʹ�.
	GM->RestartPlayer(this);
}



