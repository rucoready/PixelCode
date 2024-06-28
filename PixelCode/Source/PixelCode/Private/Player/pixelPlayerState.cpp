// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/pixelPlayerState.h"
#include "Player/StateComponent.h"
#include "MyGameModeBase.h"


void ApixelPlayerState::addUpEXP(float AcquireEXP)
{
	currentEXP += AcquireEXP;
	maxEXP();
	GM->EXPmanagement(AcquireEXP);
}

void ApixelPlayerState::maxEXP()
{
	if (Level == 1)
	{
		totalEXP = 150;
	}
	else if (Level == 2)
	{
		totalEXP = 400;
	}
	else if (Level == 3)
	{
		totalEXP = 650;
	}
	else if (Level == 4)
	{
		totalEXP = 900;
	}
	else if (Level == 5)
	{
		totalEXP = 1150;
	}
}

void ApixelPlayerState::LevelUP()
{

	Level += 1;
	
}


