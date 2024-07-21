// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PCodePlayerController.generated.h"


class AMyGameModeBase;
class UNormallyWidget;
class UPlayerStatWidget;
class ApixelPlayerState;
/**
 * 
 */
UCLASS()
class PIXELCODE_API APCodePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	

// ������ ---------------------------------------------------------------------

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// �Է� ��� ����, �Ű����� �������� Ŀ�� ǥ��
	void OpenUI(bool bOpen); 

	UPROPERTY()
	AMyGameModeBase* GM;

	UPROPERTY()
	UPlayerStatWidget* statWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UPlayerStatWidget> StatWidgetClass;

	UPROPERTY()
	UNormallyWidget* NormallyWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UNormallyWidget> NormallyWidgetClass;

	ApixelPlayerState* PlayerState;

	// UI
	void StartUI();

	void LevelUpUpdate();

	/*UFUNCTION(Server, Reliable)
	void ServerRPC_StartUI()*/;

	UFUNCTION(Client, Reliable)
	void ClientRPC_StartUI();

	bool bIsStatVisible = true;

	void StatMenu();

	void ExpUpUI();

	UFUNCTION(Server, Reliable)
	void ServerRPC_RespawnPlayer();

	

	//UFUNCTION(Server, Reliable)
	//void ServerRPC_ChangeSpectator();
// ������ �� ---------------------------------------------------------------------


};
