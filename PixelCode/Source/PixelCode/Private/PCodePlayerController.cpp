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
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/GameStateBase.h>
#include "BossLoadingWidget.h"
#include "DamageWidget.h"
#include "LoadingWidget1.h"
#include "Kismet/GameplayStatics.h"


void APCodePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GM = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
	}

	statWidget = Cast<UPlayerStatWidget>(CreateWidget(GetWorld(), StatWidgetClass));

	NormallyWidget = Cast<UNormallyWidget>(CreateWidget(GetWorld(), NormallyWidgetClass));

	pixelPlayerState = nullptr;

	if (HasAuthority())
	{
		// PlayerState
		auto temp = GetPlayerState<ApixelPlayerState>();

		if (temp == nullptr)
		{
			int iTemp = 0;
			iTemp = 1;
			iTemp = 2;
			iTemp = 3;
			iTemp = 4;
			iTemp = 5;
			iTemp = 6;

			if (pixelPlayerState == nullptr)
			{
				pixelPlayerState = Cast<ApixelPlayerState>(temp);
			}
		}

		ApixelPlayerState* CurrentPlayerState = GetPlayerState<ApixelPlayerState>();

		if (CurrentPlayerState != nullptr && pixelPlayerState == nullptr)
		{
			pixelPlayerState = CurrentPlayerState;
		}

		if (CurrentPlayerState != nullptr)
		{
			TArray<APlayerState*> psArray = GetWorld()->GetGameState()->PlayerArray;

			// psArrayCurrentPlayerStatePlayerState
			for (int i = 0; i < psArray.Num(); i++)
			{
				ApixelPlayerState* PlayerStateInArray = Cast<ApixelPlayerState>(psArray[i]);
				if (PlayerStateInArray != nullptr && PlayerStateInArray == CurrentPlayerState)
				{
					pixelPlayerState = PlayerStateInArray;
					pixelPlayerState->InitPlayerData();
					if (NormallyWidget)
					{
						//NormallyWidget->firstStatedate(pixelPlayerState);
						//NormallyWidget->currentLevelUpdate(pixelPlayerState);
						//NormallyWidget->currentExpUpdate(pixelPlayerState->currentEXP, pixelPlayerState->totalEXP);
						//pixelPlayerState->SetaddUpEXP(30.0f);
					}
					
					// ��ġ�ϴ� PlayerState
					// PlayerStateInArray
					// ��: PlayerStateInArray->SomeFunction();

					PlayerStateInArray->PlayerId;
					UE_LOG(LogTemp, Warning, TEXT("%d"), PlayerStateInArray);
					FString Message = FString::Printf(TEXT("Current On PlayerState Index: %d."), PlayerStateInArray);
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Message);


					break; // 
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("�÷��̾� ����(PlayerState)�� ã�� �� �����ϴ�."));
					UE_LOG(LogTemp, Warning, TEXT("�÷��̾� ����(PlayerState)�� ã�� �� �����ϴ�."));
				}
			}
		}
		else
		{
			pixelPlayerState = CurrentPlayerState;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("�÷��̾� ����(CurrentPlayerState)�� ã�� �� �����ϴ�."));
			UE_LOG(LogTemp, Warning, TEXT("�÷��̾� ����(CurrentPlayerState)�� ã�� �� �����ϴ�."));
		}
	}



}

// State
void APCodePlayerController::ServerSendPlayerStateToClient_Implementation(APlayerController* TargetPlayerController, ApixelPlayerState* PlayerStateData)
{
	// PlayerState 
	NetMulticastReceivePlayerStateFromServer(TargetPlayerController, PlayerStateData);
}

void APCodePlayerController::NetMulticastReceivePlayerStateFromServer_Implementation(APlayerController* TargetPlayerController, ApixelPlayerState* PlayerStateData)
{
	// PlayerState 
	if (PlayerStateData != nullptr)
	{
		// PlayerStateData�� PlayerState�� ����
		this->PlayerState = PlayerStateData;

		//  PlayerState
		UE_LOG(LogTemp, Warning, TEXT("Client received PlayerState: %s"), *PlayerState->GetPlayerName());

		// PlayerState

		//MainPlayer = Cast<APixelCodeCharacter>(TargetPlayerController->GetOwner());
		//MainPlayer->PlayerState = PlayerStateData;
	}
}

void APCodePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (NormallyWidget != nullptr && pixelPlayerState != nullptr)
	{
		//NormallyWidget->currentExpUpdate(pixelPlayerState->currentEXP, pixelPlayerState->totalEXP);

		//NormallyWidget->currentLevelUpdate(pixelPlayerState);

	}
	
		
		
		

	

}

void APCodePlayerController::OpenUI(bool bOpen)
{
	if (bOpen)
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

	// PlayerName  Replication 
	DOREPLIFETIME(APCodePlayerController, statWidget);
	DOREPLIFETIME(APCodePlayerController, NormallyWidget);


}

ApixelPlayerState* APCodePlayerController::GetPlayerStateOfOtherPlayer()
{
	//ApixelPlayerState* pixelPlayerState = nullptr;

	//if (HasAuthority())
	//{
	//	PlayerState
	//	auto temp = GetPlayerState<ApixelPlayerState>();

	//	if (temp == nullptr)
	//	{
	//		int iTemp = 0;
	//		iTemp = 1;
	//		iTemp = 2;
	//		iTemp = 3;
	//		iTemp = 4;
	//		iTemp = 5;
	//		iTemp = 6;

	//		if (pixelPlayerState == nullptr)
	//		{
	//			pixelPlayerState = Cast<ApixelPlayerState>(temp);
	//		}
	//	}

	//	ApixelPlayerState* CurrentPlayerState = GetPlayerState<ApixelPlayerState>();

	//	if (CurrentPlayerState != nullptr && pixelPlayerState == nullptr)
	//	{
	//		pixelPlayerState = CurrentPlayerState;
	//	}

	//	if (CurrentPlayerState != nullptr)
	//	{
	//		TArray<APlayerState*> psArray = GetWorld()->GetGameState()->PlayerArray;

	//		// psArray���� CurrentPlayerState�� ��ġ�ϴ� PlayerState�� ã��
	//		for (int i = 0; i < psArray.Num(); i++)
	//		{
	//			ApixelPlayerState* PlayerStateInArray = Cast<ApixelPlayerState>(psArray[i]);
	//			if (PlayerStateInArray != nullptr && PlayerStateInArray == CurrentPlayerState)
	//			{
	//				pixelPlayerState = PlayerStateInArray;

	//				return pixelPlayerState;
	//				// ��ġ�ϴ� PlayerState�� ã���� ���� ����
	//				// PlayerStateInArray�� �̿��ؼ� ���ϴ� �۾��� ����
	//				// ��: PlayerStateInArray->SomeFunction();

	//			/*	PlayerStateInArray->PlayerId;
	//				UE_LOG(LogTemp, Warning, TEXT("%d"), PlayerStateInArray);
	//				FString Message = FString::Printf(TEXT("���� �÷��̾� ������ �ε����� %d �Դϴ�."), PlayerStateInArray);
	//				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Message);*/


	//				break; // ��ġ�ϴ� ���� ã�����Ƿ� ���� ����
	//			}
	//			else
	//			{
	//				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("�÷��̾� ����(PlayerState)�� ã�� �� �����ϴ�."));
	//				UE_LOG(LogTemp, Warning, TEXT("�÷��̾� ����(PlayerState)�� ã�� �� �����ϴ�."));
	//			}
	//		}
	//	}
	//	else
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("�÷��̾� ����(CurrentPlayerState)�� ã�� �� �����ϴ�."));
	//		UE_LOG(LogTemp, Warning, TEXT("�÷��̾� ����(CurrentPlayerState)�� ã�� �� �����ϴ�."));
	//	}
	//}


	return pixelPlayerState;
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
	//	// 
	//	auto* oldPawn = GetPawn();

	//	if (oldPawn)
	//	{
	
	//		FTransform t = oldPawn->GetActorTransform();
	//		FActorSpawnParameters params;
	//		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//
	//		auto* newPawn = GetWorld()->SpawnActor<ASpectatorPawn>(GM->SpectatorClass, t, params);
	//
	//		
	//		Possess(newPawn);
	//		// oldPawn�� �ı��ϰ�ʹ�.
	//		oldPawn->Destroy();
	//		// 5���Ŀ� ServerRPC_RespawnPlayer_Implementation
	//		FTimerHandle handle;
	//		GetWorld()->GetTimerManager().SetTimer(handle, this, &APCodePlayerController::ServerRPC_RespawnPlayer_Implementation, 5, false);
	//	}
	//}

void APCodePlayerController::ServerRPC_RespawnPlayer_Implementation()
{
	
	auto* oldPawn = GetPawn();

	
	UnPossess();

	
	if (oldPawn)
	{
		oldPawn->Destroy();
	}

	
	GM->RestartPlayer(this);
}

void APCodePlayerController::ServerRPC_CreateWidgetRobbyWidget_Implementation()
{
	MulticastRPC_CreateWidgetRobbyWidget();
}

void APCodePlayerController::MulticastRPC_CreateWidgetRobbyWidget_Implementation()
{
	if (portalRobbyWidget)
	{
		WidgetInstance = CreateWidget<UPortalRobbyWidget>(this, portalRobbyWidget);
		if (WidgetInstance)
		{
			// 위젯을 화면에 추가
			WidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			bEnableClickEvents = true;
			bEnableMouseOverEvents = true;

			//MyGameMode->bIsReadyToReady=true;
		}
	}
}

void APCodePlayerController::ServerRPC_HideWidgetRobbyWidget_Implementation()
{
	MulticastRPC_HideWidgetRobbyWidget();
}

void APCodePlayerController::MulticastRPC_HideWidgetRobbyWidget_Implementation()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* BaseController = It->Get();
		APCodePlayerController* PlayerController = Cast<APCodePlayerController>(BaseController);
		if (PlayerController && WidgetInstance)
		{
			WidgetInstance->RemoveFromParent();
			PlayerController->bShowMouseCursor = false;
			PlayerController->bEnableClickEvents = false;
			PlayerController->bEnableTouchEvents = false;
		}

	}
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



void APCodePlayerController::ServerRPC_CreateWidgetLoading1_Implementation()
{
	MulticastRPC_CreateWidgetLoading1();
}

void APCodePlayerController::MulticastRPC_CreateWidgetLoading1_Implementation()
{
	if (loadingWidget1)
	{
		loadingWidget01 = CreateWidget<ULoadingWidget1>(this, loadingWidget1);
		if (loadingWidget01)
		{
			// 위젯을 화면에 추가
			loadingWidget01->AddToViewport();

			bShowMouseCursor = true;
			bEnableClickEvents = true;
			bEnableMouseOverEvents = true;

			//MyGameMode->bIsReadyToReady=true;
		}
	}
}

void APCodePlayerController::ServerRPC_HideWidgetLoading1_Implementation()
{
	MulticastRPC_HideWidgetLoading1();
}

void APCodePlayerController::MulticastRPC_HideWidgetLoading1_Implementation()
{
	if (loadingWidget1)
	{
		loadingWidget01 = CreateWidget<ULoadingWidget1>(this, loadingWidget1);
		if (loadingWidget01)
		{
			// 위젯을 화면에 추가
			loadingWidget01->RemoveFromParent();

			bShowMouseCursor = true;
			bEnableClickEvents = true;
			bEnableMouseOverEvents = true;

			//MyGameMode->bIsReadyToReady=true;
		}
	}
}

void APCodePlayerController::ServerRPC_CreateWidgetBossLoading_Implementation()
{
	MulticastRPC_CreateWidgetBossLoading();
}

void APCodePlayerController::MulticastRPC_CreateWidgetBossLoading_Implementation()
{
	if (loadingWidgetBoss)
	{
		bossLoadingWidget = CreateWidget<UBossLoadingWidget>(this, loadingWidgetBoss);
		if (bossLoadingWidget)
		{
			// 위젯을 화면에 추가
			bossLoadingWidget->AddToViewport();

			bShowMouseCursor = true;
			bEnableClickEvents = true;
			bEnableMouseOverEvents = true;

			//MyGameMode->bIsReadyToReady=true;
		}
	}
}

void APCodePlayerController::ServerRPC_HideWidgetBossLoading_Implementation()
{
	MulticastRPC_HideWidgetBossLoading();
}

void APCodePlayerController::MulticastRPC_HideWidgetBossLoading_Implementation()
{
	if (loadingWidgetBoss)
	{
		bossLoadingWidget = CreateWidget<UBossLoadingWidget>(this, loadingWidgetBoss);
		if (bossLoadingWidget)
		{
			// 위젯을 화면에 추가
			bossLoadingWidget->RemoveFromParent();

			bShowMouseCursor = true;
			bEnableClickEvents = true;
			bEnableMouseOverEvents = true;

			//MyGameMode->bIsReadyToReady=true;
		}
	}
}

void APCodePlayerController::ServerRPC_CreateDamageWidget_Implementation()
{
	MulticastRPC_CreateDamageWidget();
}

void APCodePlayerController::MulticastRPC_CreateDamageWidget_Implementation()
{
	if (damageWidget)
	{
		damageWidgets = CreateWidget<UDamageWidget>(this, damageWidget);
		if (damageWidgets)
		{
			// 위젯을 화면에 추가
			damageWidgets->AddToViewport();



			//MyGameMode->bIsReadyToReady=true;
		}
	}
}

void APCodePlayerController::ChangeRobbyWidgetButtonReady()
{
	UE_LOG(LogTemp, Warning, TEXT("TA66"));
	WidgetInstance->NormalChangeButton1();
}


