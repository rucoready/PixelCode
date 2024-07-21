// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"


class ApixelPlayerState;

/**
 * 
 */
UCLASS()
class PIXELCODE_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMyGameModeBase();

	ApixelPlayerState* PlayerState;

	float totalEXP = 0;
	float currentEXP = 0;

	void EXPmanagement(float EXP);
	
	virtual void PostLogin(APlayerController* NewPlayer) override;


};
