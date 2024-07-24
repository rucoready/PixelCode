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
		// ���� �÷��̾��� PlayerState�� ���
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

			// psArray���� CurrentPlayerState�� ��ġ�ϴ� PlayerState�� ã��
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
					
					// ��ġ�ϴ� PlayerState�� ã���� ���� ����
					// PlayerStateInArray�� �̿��ؼ� ���ϴ� �۾��� ����
					// ��: PlayerStateInArray->SomeFunction();

					PlayerStateInArray->PlayerId;
					UE_LOG(LogTemp, Warning, TEXT("%d"), PlayerStateInArray);
					FString Message = FString::Printf(TEXT("Current On PlayerState Index: %d."), PlayerStateInArray);
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Message);


					break; // ��ġ�ϴ� ���� ã�����Ƿ� ���� ����
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

// State �� �����ϱ� ���� ������ Ŭ���̾�Ʈ
void APCodePlayerController::ServerSendPlayerStateToClient_Implementation(APlayerController* TargetPlayerController, ApixelPlayerState* PlayerStateData)
{
	// Ŭ���̾�Ʈ���� PlayerState ������ ����
	NetMulticastReceivePlayerStateFromServer(TargetPlayerController, PlayerStateData);
}

void APCodePlayerController::NetMulticastReceivePlayerStateFromServer_Implementation(APlayerController* TargetPlayerController, ApixelPlayerState* PlayerStateData)
{
	// Ŭ���̾�Ʈ���� PlayerState ������ ó��
	if (PlayerStateData != nullptr)
	{
		// PlayerStateData�� PlayerState�� ����
		this->PlayerState = PlayerStateData;

		// ����: �α׿� PlayerState ���� ���
		UE_LOG(LogTemp, Warning, TEXT("Client received PlayerState: %s"), *PlayerState->GetPlayerName());

		// Ŭ���̾�Ʈ���� PlayerState�� ����ϴ� �ٸ� �Լ� ȣ�� ����

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

	// PlayerName ������ Replication ����Ʈ�� �߰�
	DOREPLIFETIME(APCodePlayerController, statWidget);
	DOREPLIFETIME(APCodePlayerController, NormallyWidget);


}

ApixelPlayerState* APCodePlayerController::GetPlayerStateOfOtherPlayer()
{
	//ApixelPlayerState* pixelPlayerState = nullptr;

	//if (HasAuthority())
	//{
	//	// ���� �÷��̾��� PlayerState�� ���
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



