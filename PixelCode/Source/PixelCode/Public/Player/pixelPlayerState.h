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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


    void SetaddUpEXP(float AcquireEXP); // float 타입으로 변경
    void LevelUP();
    void InitPlayerData();

	float GetCurrentExp() const;

    int32 GetCharacterLevel() const;

    UPROPERTY(ReplicatedUsing = OnRep_totalEXP)
    int32 totalEXP;

    UPROPERTY(ReplicatedUsing = OnRep_currentEXP)
    float currentEXP;

    UPROPERTY(Replicated)
    int32 Level;

    void maxEXP();

    UFUNCTION()
    void OnRep_totalEXP(int32 OldEXP);

    UFUNCTION()
    void OnRep_currentEXP(float OldEXP);



	UNormallyWidget* PlayerMainUI;

	UPlayerStatWidget* PlayerStatUI;

	UStateComponent* stateComp;



	UFUNCTION(BlueprintCallable, Category = "PlayerState")
    ApixelPlayerState* GetPlayerStateOfOtherPlayer(APCodePlayerController* OtherPlayerController);
	
	




	// 잠시주석
	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/* UPROPERTY(ReplicatedUsing = OnRep_addUpEXP)
	float totalEXP = 0;

	 UPROPERTY(ReplicatedUsing = OnRep_addUpEXP)
	float currentEXP = 0;

	 UPROPERTY(ReplicatedUsing = OnRep_addUpEXP)
	int32 Level;

	UFUNCTION()
	void OnRep_addUpEXP(float AcquireEXP);

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	void SetaddUpEXP(float AcquireEXP);


	int32 GetCharacterLevel() const;



	void InitPlayerData();




	void maxEXP();
	void LevelUP();

	UStateComponent* stateComp;


	AMyGameModeBase* GM;*/
	
};
