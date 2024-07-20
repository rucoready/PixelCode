// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PCodePlayerController.generated.h"


class AMyGameModeBase;
class UNormallyWidget;
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

	// �Է� ��� ����, �Ű����� �������� Ŀ�� ǥ��
	void OpenUI(bool bOpen); 

	UPROPERTY()
	AMyGameModeBase* GM;

	UPROPERTY()
	UNormallyWidget* NormallyWidget;


	UFUNCTION(Server, Reliable)
	void ServerRPC_RespawnPlayer();

	//UFUNCTION(Server, Reliable)
	//void ServerRPC_ChangeSpectator();
// ������ �� ---------------------------------------------------------------------


};
