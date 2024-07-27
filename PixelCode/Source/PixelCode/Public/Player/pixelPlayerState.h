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
class UPlayerStatWidget;
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


    void SetaddUpEXP(float AcquireEXP); // float 타입으로 변경
    void LevelUP();
    void InitPlayerData();

	float GetCurrentExp() const;

    int32 GetCharacterLevel() const;

    UPROPERTY()
    int32 totalEXP;

    UPROPERTY()
    float currentEXP;

    UPROPERTY()
    int32 Level;

    void maxEXP();

    UFUNCTION()
    void OnRep_totalEXP(int32 OldEXP);

    UFUNCTION()
    void OnRep_currentEXP(float OldEXP);
};
