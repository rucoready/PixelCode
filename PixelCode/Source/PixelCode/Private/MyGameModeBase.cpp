// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "Player/pixelPlayerState.h"
#include "Player/PixelCodeCharacter.h" 
#include "PortalRobbyWidget.h"
#include "PCodePlayerController.h" 
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "PortalCollision.h"
#include "PCodePlayerController.h"
#include "EngineUtils.h" 
#include "Kismet/GameplayStatics.h"

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = APixelCodeCharacter::StaticClass();
	PlayerControllerClass = APCodePlayerController::StaticClass();
	PlayerStateClass = ApixelPlayerState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;

}

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	portalWidget = CreateWidget<UPortalRobbyWidget>(GetWorld(), UPortalRobbyWidget::StaticClass());
	if (portalWidget)
	{
		portalWidget;
	}
	for (TActorIterator<APortalCollision> It(GetWorld()); It; ++It)
	{
		portalCollision = *It;
		if (portalCollision)
		{
			//portalCollision->ServerRPC_ShowRobbyWidget();
			
			//break; // 여러 개의 액터가 있을 경우, 첫 번째 액터만 처리하려면 break
		}
	}

	
	// PlayerController가 null이 아닌지 확인
	pc = Cast<APCodePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	{

	}
		
	
		
}

void AMyGameModeBase::EXPmanagement(float EXP, ApixelPlayerState* PlayerState)
{
	currentEXP += EXP;

	if (PlayerState->totalEXP <= currentEXP)
	{
		PlayerState->LevelUP();
		PlayerState->currentEXP = 0;
	}

}

void AMyGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
  	if (bIsReadyToReady&&!onceDo)
  	{
		onceDo = true;
		ServerRPC_ChangeeReadyButtonUI();
		

  	}
}

void AMyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto NewPlayerState = Cast<ApixelPlayerState>(NewPlayer->PlayerState);
	
	if (NewPlayerState != nullptr)
	{
		NewPlayerState->InitPlayerData();
	}

}

void AMyGameModeBase::ServerRPC_ChangeeReadyButtonUI_Implementation()
{
	pc->ChangeRobbyWidgetButtonReady();
}
