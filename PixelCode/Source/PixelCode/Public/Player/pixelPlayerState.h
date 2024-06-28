// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Engine/DataTable.h"
#include "pixelPlayerState.generated.h"

class UStateComponent;
class AMyGameModeBase;
class UDataTable;


/**
 * 
 */
// struct ApixelPlayerState : public FTableRowBase
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EXP")
//	float MAXEXP;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EXP")
//	int32 PlayerLevel;
//};



UCLASS()
class PIXELCODE_API ApixelPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	
	float totalEXP = 0;
	float currentEXP = 0;
	int32 Level = 1;

	void addUpEXP(float AcquireEXP);
	void maxEXP();
	void LevelUP();
	
	UStateComponent* stateComp;
	AMyGameModeBase* GM;
	
};
