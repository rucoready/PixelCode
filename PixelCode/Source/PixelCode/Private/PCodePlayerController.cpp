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
#include "BossEnterWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MyMapWidget.h"
#include "MyMapLodingWidget.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>


void APCodePlayerController::BeginPlay()
{
	Super::BeginPlay();

	pixelPlayerState = nullptr;

	if (HasAuthority())
	{
		GM = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
	}
	MainPlayer = Cast<APixelCodeCharacter>(this->GetPawn());

	statWidget = Cast<UPlayerStatWidget>(CreateWidget(GetWorld(), StatWidgetClass));

	NormallyWidget = Cast<UNormallyWidget>(CreateWidget(GetWorld(), NormallyWidgetClass));

	PlayerStartWidget();

	// 플레이어 스테이트가 유효한지 검사하고 유효한 경우에만 Setup_PC 함수를 호출

	if (ValidatePlayerState())
	{
		PlayerWidgetUpdate();
	}
	
}

void APCodePlayerController::FullExp()
{
	NormallyWidget->firstStatedate(pixelPlayerState);
}

void APCodePlayerController::PlayerLevelUp()
{
	pixelPlayerState->SetaddUpEXP(30.0f);
}

bool APCodePlayerController::ValidatePlayerState()
{
	// Ensures that the player state is Valid before we setup the PlayerController
	// 플레이어 컨트롤러를 설정하기 전에, 플레이어 스테이트가 유효한지 보장한다.
	FTimerHandle TimerHandle_PlayerStateCheck;

	if (PlayerState->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("0 APC_Gameplay::PlayerState->IsValid"));
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("0 APC_Gameplay::PlayerState->Is Not Valid"));
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_PlayerStateCheck, [&]() {
			if (ValidatePlayerState())
			{
				UE_LOG(LogTemp, Warning, TEXT("0 APC_Gameplay::PlayerState 다시 validate!!!!!!!!"));
				PlayerWidgetUpdate();
			}
			}, 0.2f, false);

		return false;
	}
}

void APCodePlayerController::PlayerWidgetUpdate()
{
	pixelPlayerState = Cast<ApixelPlayerState>(PlayerState);

		if (pixelPlayerState)
		{
			NormallyWidget->currentExpUpdate(pixelPlayerState);
			NormallyWidget->firstStatedate(pixelPlayerState);
;			statWidget->UpdateLevel(pixelPlayerState);
		}
}

void APCodePlayerController::PlayerStartWidget()
{
	MainPlayer = Cast<APixelCodeCharacter>(this->GetPawn());
	if (IsLocalController())
	{

		// 시작
		if (StatWidgetClass)
		{
			statWidget = Cast<UPlayerStatWidget>(CreateWidget(GetWorld(), StatWidgetClass));
			statWidget = statWidget;
			if (statWidget != nullptr)
			{
				statWidget->AddToViewport(1);
				statWidget->SetVisibility(ESlateVisibility::Collapsed);
				UE_LOG(LogTemp, Warning, TEXT("NormalAuth"));
			}

		}

		if (NormallyWidgetClass)
		{
			NormallyWidget = Cast<UNormallyWidget>(CreateWidget(GetWorld(), NormallyWidgetClass));
			NormallyWidget = NormallyWidget;
			if (NormallyWidget != nullptr && MainPlayer != nullptr)
			{
				NormallyWidget->AddToViewport(-1);
				NormallyWidget->SetVisibility(ESlateVisibility::Visible);
				NormallyWidget->currentStatUpdate(MainPlayer->stateComp);
				UE_LOG(LogTemp, Warning, TEXT("NormalAuth"));
			}
		}
	}
}

void APCodePlayerController::PlayerStatWidget()
{
	if (!MainPlayer->bIsJump)
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

}

void APCodePlayerController::PlayerStopWidget()
{
	NormallyWidget->SetActiveStopWidgetUI(true);
}

void APCodePlayerController::PlayerBaseSkillWidget()
{
	NormallyWidget->SetBaseSkillWidget(true);
}

void APCodePlayerController::PlayerSwordSkillWidget()
{
	NormallyWidget->SetSwordSkillWidget(true);
}

void APCodePlayerController::PlayerMageSkillWidget()
{
	NormallyWidget->SetMageSkillWidget(true);
}

void APCodePlayerController::PlayerDieWidget()
{
	if (NormallyWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("RespawnOn22222"));
		NormallyWidget->SetActiveGameOverUI(true);
	}
}




void APCodePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (pixelPlayerState != nullptr && NormallyWidget != nullptr)
	{
		NormallyWidget->currentExpUpdate(pixelPlayerState);
		NormallyWidget->currentLevelUpdate(pixelPlayerState);
	}
	if (MainPlayer != nullptr && NormallyWidget != nullptr)
	{
		NormallyWidget->currentStatUpdate(MainPlayer->stateComp);
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

void APCodePlayerController::CreateWidgetBossEnterWidget()
{
	
	if (bossEnterWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("CALL4455"));
		bossEnterWidgets = CreateWidget<UBossEnterWidget>(this, bossEnterWidget);
		if (bossEnterWidgets)
		{
			// 위젯을 화면에 추가
			bossEnterWidgets->AddToViewport();
			
			bShowMouseCursor = true;
			bEnableClickEvents = true;
			bEnableMouseOverEvents = true;
		}
	}
}

// =========================================== 요한 =========================================================

void APCodePlayerController::ServerRPC_CreateWidgetMyMap_Implementation()
{
	MulticastRPC_CreateWidgetMyMap();
}

void APCodePlayerController::MulticastRPC_CreateWidgetMyMap_Implementation()
{

	if (LoadingWidgetMyMap)
	{
		MyMapLoadingWidget = CreateWidget<UMyMapLodingWidget>(this, LoadingWidgetMyMap);
		if (MyMapLoadingWidget)
		{
			// 위젯을 화면에 추가
			MyMapLoadingWidget->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("111112222223333344444555666777888999"));
			bShowMouseCursor = true;
			bEnableClickEvents = true;
			bEnableMouseOverEvents = true;

			//MyGameMode->bIsReadyToReady=true;
		}
	}
}

void APCodePlayerController::ServerRPC_HideWidgetMyMap_Implementation()
{
	MulticastRPC_HideWidgetMyMapLoding();
}

void APCodePlayerController::MulticastRPC_HideWidgetMyMapLoding_Implementation()
{
	if (LoadingWidgetMyMap)
	{
		MyMapLoadingWidget = CreateWidget<UMyMapLodingWidget>(this, LoadingWidgetMyMap);
		if (MyMapLoadingWidget)
		{
			// 위젯을 화면에 추가
			MyMapLoadingWidget->RemoveFromParent();

			bShowMouseCursor = true;
			bEnableClickEvents = true;
			bEnableMouseOverEvents = true;

			//MyGameMode->bIsReadyToReady=true;
		}
	}
}

void APCodePlayerController::CreateWidgetMyMAPs()
{
	if (MyMapEnterWidget)
	{
		//UE_LOG(LogTemp, Warning, TEXT("CALL4455"));
		MyMapEnterWidgets = CreateWidget<UMyMapWidget>(this, MyMapEnterWidget);
		if (MyMapEnterWidgets)
		{
			// 위젯을 화면에 추가
			MyMapEnterWidgets->AddToViewport();

			bShowMouseCursor = true;
			bEnableClickEvents = true;
			bEnableMouseOverEvents = true;
		}
	}

}


