// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "Player/pixelPlayerState.h"
#include "Player/PixelCodeCharacter.h" // 없어도 됨?
#include "PCodePlayerController.h" // 없어도 됨?

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = APixelCodeCharacter::StaticClass();
	PlayerControllerClass = APCodePlayerController::StaticClass();
	PlayerStateClass = ApixelPlayerState::StaticClass();

}

void AMyGameModeBase::EXPmanagement(float EXP)
{
	currentEXP += EXP;

	if (PlayerState->totalEXP <= currentEXP)
	{
		PlayerState->LevelUP();
	}

}

void AMyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto NewPlayerState = Cast<ApixelPlayerState>(NewPlayer->PlayerState);
	// 플레이어 데이터 받기
	if (NewPlayerState != nullptr)
	{
		NewPlayerState->InitPlayerData();
	}

}
