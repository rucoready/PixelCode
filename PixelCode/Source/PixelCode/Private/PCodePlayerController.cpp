// Fill out your copyright notice in the Description page of Project Settings.


#include "PCodePlayerController.h"
#include "MyGameModeBase.h"
#include "GameFramework/SpectatorPawn.h"
#include "Player/pixelPlayerState.h"
#include "Player/PlayerStatWidget.h"
#include "Player/Widget/NormallyWidget.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h>
#include "Player/PixelCodeCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>

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


void APCodePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// PlayerName 변수를 Replication 리스트에 추가
	DOREPLIFETIME(APCodePlayerController, statWidget);
	DOREPLIFETIME(APCodePlayerController, NormallyWidget);
		

}



//void APCodePlayerController::ServerRPC_StartUI_Implementation()
//{
//	StartUI();
//	ClientRPC_StartUI();
////	/*statWidget = Cast<UPlayerStatWidget>(CreateWidget(GetWorld(), StatWidgetClass));
////	NormallyWidget = Cast<UNormallyWidget>(CreateWidget(GetWorld(), NormallyWidgetClass));
////	PlayerState = Cast<ApixelPlayerState>(GetPlayerState<ApixelPlayerState>());*/
////	
////	/*if (HasAuthority())
////	{
////	statWidget->UpdateLevel(PlayerState->Level);
////		ClientRPC_StartUI();*/
////	
////	
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

//
//void APCodePlayerController::ClientRPC_StartUI_Implementation()
//{
//	StartUI();
//
//
//
//	//NormallyWidget->currentExpUpdate(PlayerState->currentEXP, PlayerState->totalEXP);
//}












//void APCodePlayerController::ServerRPC_ChangeSpectator_Implementation()
//{
//	// 현재 주인공을 기억하고싶다.
//	auto* oldPawn = GetPawn();
//	// 만약 oldPawn이 있다면
//	if (oldPawn)
//	{
//		// 관전자를 만들어서 
//		FTransform t = oldPawn->GetActorTransform();
//		FActorSpawnParameters params;
//		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//		auto* newPawn = GetWorld()->SpawnActor<ASpectatorPawn>(GM->SpectatorClass, t, params);
//
//		// 빙의 하고싶다.
//		Possess(newPawn);
//		// oldPawn은 파괴하고싶다.
//		oldPawn->Destroy();
//		// 5초후에 ServerRPC_RespawnPlayer_Implementation를 호출하고싶다.
//		FTimerHandle handle;
//		GetWorld()->GetTimerManager().SetTimer(handle, this, &APCodePlayerController::ServerRPC_RespawnPlayer_Implementation, 5, false);
//	}
//}

void APCodePlayerController::ServerRPC_RespawnPlayer_Implementation()
{
	// 현재 주인공을 기억하고싶다.
	auto* oldPawn = GetPawn();

	// 빙의를 해제 하고싶다.
	UnPossess();

	// 기억했던 주인공을 파괴 하고싶다.
	if (oldPawn)
	{
		oldPawn->Destroy();
	}

	// GameMode의 재시작 함수를 호출하고싶다.
	GM->RestartPlayer(this);
}



