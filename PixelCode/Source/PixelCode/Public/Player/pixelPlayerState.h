// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Engine/DataTable.h"
#include "pixelPlayerState.generated.h"

class UStateComponent;
class AMyGameModeBase;
class UDataTable;
class UNormallyWidget;

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
	
	ApixelPlayerState();

	float totalEXP = 0;
	float currentEXP = 0;

	UPROPERTY(Transient) // 네트워크 통해 전송 x, 게임저장파일에 저장 x
	int32 Level;


	int32 GetCharacterLevel() const;

	void InitPlayerData();

	

	void addUpEXP(float AcquireEXP);
	void maxEXP();
	void LevelUP();
	
	UStateComponent* stateComp;

	UNormallyWidget* PlayerMainUI;

	AMyGameModeBase* GM;
	
};
