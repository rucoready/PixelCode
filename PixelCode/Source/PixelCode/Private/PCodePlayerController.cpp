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
		// 현재 플레이어의 PlayerState를 얻기
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

			// psArray에서 CurrentPlayerState와 일치하는 PlayerState를 찾기
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
					
					// 일치하는 PlayerState를 찾았을 때의 로직
					// PlayerStateInArray를 이용해서 원하는 작업을 수행
					// 예: PlayerStateInArray->SomeFunction();

					PlayerStateInArray->PlayerId;
					UE_LOG(LogTemp, Warning, TEXT("%d"), PlayerStateInArray);
					FString Message = FString::Printf(TEXT("Current On PlayerState Index: %d."), PlayerStateInArray);
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Message);


					break; // 일치하는 것을 찾았으므로 루프 종료
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("플레이어 상태(PlayerState)를 찾을 수 없습니다."));
					UE_LOG(LogTemp, Warning, TEXT("플레이어 상태(PlayerState)를 찾을 수 없습니다."));
				}
			}
		}
		else
		{
			pixelPlayerState = CurrentPlayerState;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("플레이어 상태(CurrentPlayerState)를 찾을 수 없습니다."));
			UE_LOG(LogTemp, Warning, TEXT("플레이어 상태(CurrentPlayerState)를 찾을 수 없습니다."));
		}
	}



}

// State 를 전달하기 위한 서버랑 클라이언트
void APCodePlayerController::ServerSendPlayerStateToClient_Implementation(APlayerController* TargetPlayerController, ApixelPlayerState* PlayerStateData)
{
	// 클라이언트에게 PlayerState 데이터 전달
	NetMulticastReceivePlayerStateFromServer(TargetPlayerController, PlayerStateData);
}

void APCodePlayerController::NetMulticastReceivePlayerStateFromServer_Implementation(APlayerController* TargetPlayerController, ApixelPlayerState* PlayerStateData)
{
	// 클라이언트에서 PlayerState 데이터 처리
	if (PlayerStateData != nullptr)
	{
		// PlayerStateData를 PlayerState에 설정
		this->PlayerState = PlayerStateData;

		// 예시: 로그에 PlayerState 정보 출력
		UE_LOG(LogTemp, Warning, TEXT("Client received PlayerState: %s"), *PlayerState->GetPlayerName());

		// 클라이언트에서 PlayerState를 사용하는 다른 함수 호출 예시

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

	// PlayerName 변수를 Replication 리스트에 추가
	DOREPLIFETIME(APCodePlayerController, statWidget);
	DOREPLIFETIME(APCodePlayerController, NormallyWidget);


}

ApixelPlayerState* APCodePlayerController::GetPlayerStateOfOtherPlayer()
{
	//ApixelPlayerState* pixelPlayerState = nullptr;

	//if (HasAuthority())
	//{
	//	// 현재 플레이어의 PlayerState를 얻기
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

	//		// psArray에서 CurrentPlayerState와 일치하는 PlayerState를 찾기
	//		for (int i = 0; i < psArray.Num(); i++)
	//		{
	//			ApixelPlayerState* PlayerStateInArray = Cast<ApixelPlayerState>(psArray[i]);
	//			if (PlayerStateInArray != nullptr && PlayerStateInArray == CurrentPlayerState)
	//			{
	//				pixelPlayerState = PlayerStateInArray;

	//				return pixelPlayerState;
	//				// 일치하는 PlayerState를 찾았을 때의 로직
	//				// PlayerStateInArray를 이용해서 원하는 작업을 수행
	//				// 예: PlayerStateInArray->SomeFunction();

	//			/*	PlayerStateInArray->PlayerId;
	//				UE_LOG(LogTemp, Warning, TEXT("%d"), PlayerStateInArray);
	//				FString Message = FString::Printf(TEXT("현재 플레이어 상태의 인덱스는 %d 입니다."), PlayerStateInArray);
	//				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Message);*/


	//				break; // 일치하는 것을 찾았으므로 루프 종료
	//			}
	//			else
	//			{
	//				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("플레이어 상태(PlayerState)를 찾을 수 없습니다."));
	//				UE_LOG(LogTemp, Warning, TEXT("플레이어 상태(PlayerState)를 찾을 수 없습니다."));
	//			}
	//		}
	//	}
	//	else
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("플레이어 상태(CurrentPlayerState)를 찾을 수 없습니다."));
	//		UE_LOG(LogTemp, Warning, TEXT("플레이어 상태(CurrentPlayerState)를 찾을 수 없습니다."));
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



