// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "Player/pixelPlayerState.h"

void AMyGameModeBase::EXPmanagement(float EXP)
{
	currentEXP += EXP;

	if (PlayerState->totalEXP <= currentEXP)
	{
		PlayerState->LevelUP();
	}

}
