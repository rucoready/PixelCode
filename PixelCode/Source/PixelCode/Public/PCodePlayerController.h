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
	
	

// 리스폰 ---------------------------------------------------------------------

	virtual void BeginPlay() override;

	// 입력 모드 설정, 매개변수 기점으로 커서 표현
	void OpenUI(bool bOpen); 

	UPROPERTY()
	AMyGameModeBase* GM;

	UPROPERTY()
	UNormallyWidget* NormallyWidget;


	UFUNCTION(Server, Reliable)
	void ServerRPC_RespawnPlayer();

	//UFUNCTION(Server, Reliable)
	//void ServerRPC_ChangeSpectator();
// 리스폰 끝 ---------------------------------------------------------------------


};
