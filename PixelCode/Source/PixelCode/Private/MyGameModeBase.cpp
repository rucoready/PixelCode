// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "Player/pixelPlayerState.h"
#include "Player/PixelCodeCharacter.h" // ��� ��?
#include "PCodePlayerController.h" // ��� ��?

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
	// �÷��̾� ������ �ޱ�
	if (NewPlayerState != nullptr)
	{
		NewPlayerState->InitPlayerData();
	}

}
