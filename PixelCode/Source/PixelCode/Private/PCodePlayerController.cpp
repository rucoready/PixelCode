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
#include "Player/StateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/Pooling/PlayerObjectPoolManager.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include "Player/PlayerOrganism.h"


void APCodePlayerController::BeginPlay()
{
	Super::BeginPlay();

	//pixelPlayerState = nullptr;

	if (HasAuthority())
	{
		GM = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
	}

	TArray<AActor*> allActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), allActors);

	for(auto Temp : allActors)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Name : %s"), *Temp->GetActorNameOrLabel()));
		
	}


	MainPlayer = Cast<APixelCodeCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	StatComponent = Cast<UStateComponent>(MainPlayer->stateComp);

	statWidget = Cast<UPlayerStatWidget>(CreateWidget(GetWorld(), StatWidgetClass));

	NormallyWidget = Cast<UNormallyWidget>(CreateWidget(GetWorld(), NormallyWidgetClass));
	
	ClientRPC_PlayerStartWidget();
	

	//PlayerBeginWidget();


	if (ValidatePlayerState())
	{
		PlayerWidgetUpdate();
	}

		// 플레이어 스테이트가 유효한지 검사하고 유효한 경우에만 Setup_PC 함수를 호출
		 // ObjectPoolManager 초기화
		// 예시: World에서 AObjectPoolManager 찾기
		for (TActorIterator<APlayerObjectPoolManager> It(GetWorld()); It; ++It)
		{
			ObjectPoolManager = *It;
			break; // 첫 번째 찾은 오브젝트 풀 매니저 사용
		}

		if (!ObjectPoolManager)
		{
			// 오브젝트 풀 매니저가 없는 경우 에러 처리
			//UE_LOG(LogTemp, Error, TEXT("ObjectPoolManager를 찾을 수 없습니다!"));
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
		//UE_LOG(LogTemp, Warning, TEXT("0 APC_Gameplay::PlayerState->IsValid"));
		return true;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("0 APC_Gameplay::PlayerState->Is Not Valid"));
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_PlayerStateCheck, [&]() {
			if (ValidatePlayerState())
			{
				//UE_LOG(LogTemp, Warning, TEXT("0 APC_Gameplay::PlayerState 다시 validate!!!!!!!!"));
				PlayerWidgetUpdate();
			}
			}, 0.2f, false);

		return false;
	}
}


void APCodePlayerController::PlayerWidgetUpdate()
{
	pixelPlayerState = Cast<ApixelPlayerState>(PlayerState);

		if (pixelPlayerState && NormallyWidget != nullptr && statWidget != nullptr)
		{
			NormallyWidget->currentExpUpdate(pixelPlayerState);
			NormallyWidget->firstStatedate(pixelPlayerState);
;			statWidget->UpdateLevel(pixelPlayerState);
		}
}

void APCodePlayerController::ClientRPC_PlayerStartWidget_Implementation()
{

	// 시작
	if (StatWidgetClass)
	{
		if (statWidget != nullptr)
		{
			statWidget->AddToViewport(1);
			statWidget->SetVisibility(ESlateVisibility::Collapsed);
			//UE_LOG(LogTemp, Warning, TEXT("NormalAuth"));
		}

	}

	if (NormallyWidgetClass)
	{
		if (NormallyWidget != nullptr)
		{
			NormallyWidget->AddToViewport(-1);
			NormallyWidget->SetVisibility(ESlateVisibility::Visible);
			//UE_LOG(LogTemp, Warning, TEXT("NormalAuth"));
		}
	}


}

//void APCodePlayerController::PlayerStartWidget()
//{
//	
//	statWidget = Cast<UPlayerStatWidget>(CreateWidget(GetWorld(), StatWidgetClass));
//
//	NormallyWidget = Cast<UNormallyWidget>(CreateWidget(GetWorld(), NormallyWidgetClass));
//
//	// 시작
//	if (StatWidgetClass)
//	{
//		if (statWidget != nullptr)
//		{
//			statWidget->AddToViewport(1);
//			statWidget->SetVisibility(ESlateVisibility::Collapsed);
//			UE_LOG(LogTemp, Warning, TEXT("NormalAuth"));
//		}
//
//	}
//
//	if (NormallyWidgetClass)
//	{
//		if (NormallyWidget != nullptr)
//		{
//			NormallyWidget->AddToViewport(-1);
//			NormallyWidget->SetVisibility(ESlateVisibility::Visible);
//			UE_LOG(LogTemp, Warning, TEXT("NormalAuth"));
//		}
//	}
//
//
//}

//void APCodePlayerController::PlayerBeginWidget()
//{
//	// 시작
//	if (StatWidgetClass)
//	{
//		if (statWidget != nullptr)
//		{
//			statWidget->AddToViewport(1);
//			statWidget->SetVisibility(ESlateVisibility::Collapsed);
//			UE_LOG(LogTemp, Warning, TEXT("NormalAuth"));
//		}
//
//	}
//
//	if (NormallyWidgetClass)
//	{
//		if (NormallyWidget != nullptr)
//		{
//			NormallyWidget->AddToViewport(-1);
//			NormallyWidget->SetVisibility(ESlateVisibility::Visible);
//			UE_LOG(LogTemp, Warning, TEXT("NormalAuth"));
//		}
//	}
//}

void APCodePlayerController::PlayerStatWidget()
{
	if (!MainPlayer->bIsJump)
	{
		if (bIsStatVisible)
		{
			statWidget->DisplayStat();
			bIsStatVisible = false;
			//UE_LOG(LogTemp, Warning, TEXT("StatOn"));
		}

		else
		{
			statWidget->HideStat();
			bIsStatVisible = true;
			//UE_LOG(LogTemp, Warning, TEXT("StatOff"));
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
		//UE_LOG(LogTemp, Warning, TEXT("RespawnDieWidget"));
		NormallyWidget->SetActiveGameOverUI(true);
	}
}

//void APCodePlayerController::PlayerRespawn()
//{
//	MainPlayer->stateComp->InitStat();
//
//	ServerRPC_PlayerRespawn();
//}
//
//void APCodePlayerController::ServerRPC_PlayerRespawn_Implementation()
//{
//	MainPlayer->SetActorLocation(MainPlayer->GetActorLocation());
//	MainPlayer->EnableInput(this);	
//	MainPlayer->bDead = false;
//	ClientRPC_PlayerRespawn();
//}
//
//void APCodePlayerController::ClientRPC_PlayerRespawn_Implementation()
//{
//	SetInputMode(FInputModeGameOnly());
//}




void APCodePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (MainPlayer == nullptr)
	{
		MainPlayer = Cast<APixelCodeCharacter>(GetPawn());
		if (MainPlayer == nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("MainPlayer is still nullptr in Tick!"));
			return;
		}
	}

	if (pixelPlayerState != nullptr && NormallyWidget != nullptr)
	{
		NormallyWidget->currentExpUpdate(pixelPlayerState);
		NormallyWidget->currentLevelUpdate(pixelPlayerState);
	}
	
	if (MainPlayer->stateComp != nullptr && NormallyWidget != nullptr && MainPlayer != nullptr)
	{
		NormallyWidget->currentStatUpdate(StatComponent);
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

void APCodePlayerController::HandleCharacterDeath()
{
	if (ObjectPoolManager)
	{
		UnPossess(); // 조종 중지

		// 캐릭터 사망 애니메이션 처리
		// ...


		//UE_LOG(LogTemp, Warning, TEXT("UnPossess!"));
	}
	//SpawnCharacterAtLocation(MainPlayer->GetActorLocation());
}

void APCodePlayerController::ClientRPC_PlayerSpawnWidget_Implementation()
{
	StatComponent->InitStat();
}

void APCodePlayerController::ServerRPC_SpawnCharacterAtLocation_Implementation()
{
	APixelCodeCharacter* PooledCharacter = ObjectPoolManager->GetPooledCharacter();
	SpawnCharacterAtLocation(PooledCharacter);

}

//void APCodePlayerController::Server_SpawnCharacterAtLocation_Implementation(APixelCodeCharacter* CharacterToSpawn, const FVector& Location)
//{
//	SpawnCharacterAtLocation(CharacterToSpawn,Location);
//}

void APCodePlayerController::SpawnCharacterAtLocation(APixelCodeCharacter* PooledCharacter)
{
	//SpawnCharacterAtLocation(MainPlayer->GetActorLocation());
	if (ObjectPoolManager)
	{
		// 오브젝트 풀에서 사용 가능한 캐릭터 가져오기
		
		if (PooledCharacter)
		{
			PooledCharacter->bPoss = true;
			PooledCharacter->SetActorLocation(PooledCharacter->GetActorLocation()); // 위치 설정
			PooledCharacter->SetActorHiddenInGame(false); // 게임에서 표시
			PooledCharacter->SetActorEnableCollision(true); // 충돌 활성화
			PooledCharacter->GetCharacterMovement()->Activate(); // 움직임 활성화
			StatComponent = PooledCharacter->stateComp;
			
			ClientRPC_PlayerSpawnWidget();

			NormallyWidget = this->NormallyWidget;
			NormallyWidget->bPlayerDie = true;

			

			//Possess(PooledCharacter);
		
			if (HasAuthority())
			{
				 //서버 권한이 있는 경우에는 바로 호출
				Possess(PooledCharacter);
			}
			else
			{
				 //서버 권한이 없는 경우에는 서버에 요청
				Server_SpawnAndPossessCharacter(PooledCharacter);
			}
			//Server_SpawnAndPossessCharacter(PooledCharacter, Location);
			//Possess(PooledCharacter); // 컨트롤러가 캐릭터를 조종
			//UE_LOG(LogTemp, Warning, TEXT("Possess!"));
		}
	}
}

void APCodePlayerController::DeleteCharacter(APixelCodeCharacter* APlayerchar, const FVector& Location)
{
	// 일정 시간 후 캐릭터 숨기기
	GetWorld()->GetTimerManager().SetTimerForNextTick([this, APlayerchar]()
		{
			APlayerchar->motionState = ECharacterMotionState::Idle;

			APlayerchar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			APlayerchar->GetMesh()->SetCollisionProfileName("CharacterMesh");
			APlayerchar->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			//APlayerchar->GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, APlayerchar->AM_DeathMontage);

			ServerRPC_SpawnCharacterAtLocation();
			ObjectPoolManager->ReturnPooledCharacter(APlayerchar); // 캐릭터 반환
			
			
		});
}


void APCodePlayerController::Server_SpawnAndPossessCharacter_Implementation(APixelCodeCharacter* CharacterToSpawn)
{
	//SpawnCharacterAtLocation(CharacterToSpawn, Location);
	Possess(CharacterToSpawn);
}


void APCodePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// PlayerName  Replication 
	
	// 오브젝트 풀 관리자 객체를 네트워크로 복제합니다.
	DOREPLIFETIME(APCodePlayerController, ObjectPoolManager);

}


//void APCodePlayerController::ServerRPC_RespawnPlayer_Implementation()
//{
//
//	auto* oldPawn = GetPawn();
//
//	UE_LOG(LogTemp, Warning, TEXT("Possess"));
//
//	if (IsLocalController())
//	{
//		NormallyWidget->RemoveFromParent();
//		statWidget->RemoveFromParent();
//	}
//	
//	UnPossess();
//
//	// 시작
//	
//	if (oldPawn)
//	{
//		oldPawn->Destroy();
//	}
//
//
//	GM->RestartPlayer(this);
//}

void APCodePlayerController::ServerRPC_CreateWidgetRobbyWidget_Implementation()
{
	MulticastRPC_CreateWidgetRobbyWidget();
}

void APCodePlayerController::MulticastRPC_CreateWidgetRobbyWidget_Implementation()
{
	if (HasAuthority())
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

				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(WidgetInstance->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				SetInputMode(InputMode);

				PlayerController = GetWorld()->GetFirstPlayerController();
				if (PlayerController)
				{
					PlayerController->SetIgnoreLookInput(true);
					PlayerController->SetIgnoreMoveInput(true);
				}

			}
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
		PlayerController = Cast<APCodePlayerController>(BaseController);
		if (PlayerController && WidgetInstance)
		{
			WidgetInstance->RemoveFromParent();
			PlayerController->bShowMouseCursor = false;
			PlayerController->bEnableClickEvents = false;
			PlayerController->bEnableTouchEvents = false;

			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);


			if (PlayerController)
			{
				PlayerController->SetIgnoreLookInput(false);
				PlayerController->SetIgnoreMoveInput(false);
			}
		}

	}
}

	//void APCodePlayerController::ServerRPC_ChangeSpectator_Implementation()
	//{
	//	
	//	// 재시작요청을 하면 서버RPC로 관전자를 생성해서 빙의하라고한다.
	//	// 관전자를 생성하고
	//	auto* oldPawn = GetPawn();

	//	if (oldPawn)
	//	{
	//	
	//		FTransform t = oldPawn->GetActorTransform();
	//		FActorSpawnParameters params;
	//		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//
	//		auto* newPawn = GetWorld()->SpawnActor<ASpectatorPawn>(GM->SpectatorClass, t, params);
	//		
	//		// 관전자로 빙의
	//		Possess(newPawn);
	//		
	//		oldPawn->Destroy();
	//
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
	//UE_LOG(LogTemp, Warning, TEXT("TA66"));
	WidgetInstance->NormalChangeButton1();
}

void APCodePlayerController::CreateWidgetBossEnterWidget()
{
	if (HasAuthority())
	{
		if (bossEnterWidget)
		{
			//UE_LOG(LogTemp, Warning, TEXT("CALL4455"));
			bossEnterWidgets = CreateWidget<UBossEnterWidget>(this, bossEnterWidget);
			if (bossEnterWidgets)
			{
				// 위젯을 화면에 추가
				bossEnterWidgets->AddToViewport();

				bShowMouseCursor = true;
				bEnableClickEvents = true;
				bEnableMouseOverEvents = true;

				PlayerController = GetWorld()->GetFirstPlayerController();
				if (PlayerController)
				{
					PlayerController->SetIgnoreLookInput(true);
					PlayerController->SetIgnoreMoveInput(true);
				}
			}
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
			//UE_LOG(LogTemp, Warning, TEXT("111112222223333344444555666777888999"));
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

void APCodePlayerController::ServerRPC_HidMyMap_Implementation()
{
	MulticastRPC_HidMyMap();
}

void APCodePlayerController::MulticastRPC_HidMyMap_Implementation()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* BaseController = It->Get();
		PlayerController = Cast<APCodePlayerController>(BaseController);
		if (PlayerController && MyMapEnterWidgets)
		{
			MyMapEnterWidgets->RemoveFromParent();
			PlayerController->bShowMouseCursor = false;
			PlayerController->bEnableClickEvents = false;
			PlayerController->bEnableTouchEvents = false;

			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);

			PlayerController->SetIgnoreLookInput(false);
			PlayerController->SetIgnoreMoveInput(false);
		}

	}
}

// ==================== 요한 =====================================

void APCodePlayerController::ServerRPC_HideLastBossPortal_Implementation()
{
	MulticastRPC_HideLastBossPortal();
}

void APCodePlayerController::MulticastRPC_HideLastBossPortal_Implementation()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* BaseController = It->Get();
		PlayerController = Cast<APCodePlayerController>(BaseController);
		if (PlayerController && bossEnterWidgets)
		{
			bossEnterWidgets->RemoveFromParent();
			PlayerController->bShowMouseCursor = false;
			PlayerController->bEnableClickEvents = false;
			PlayerController->bEnableTouchEvents = false;

			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);

			PlayerController->SetIgnoreLookInput(false);
			PlayerController->SetIgnoreMoveInput(false);
		}

	}
}
