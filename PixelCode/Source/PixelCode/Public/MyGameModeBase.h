// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PortalRobbyWidget.h"
#include "PCodePlayerController.h"
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


	float totalEXP = 0;
	float currentEXP = 0;


	

	UPROPERTY()
    class UPortalRobbyWidget* portalWidget;

	class APortalCollision* portalCollision;

	virtual void BeginPlay() override;

	void EXPmanagement(float EXP, ApixelPlayerState* PlayerState);
	
	virtual void Tick(float DeltaTime) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	bool bReady1 = false;
	
	bool bReady2 = false;

	UPROPERTY()
    TArray<UPortalRobbyWidget*> PortalRobbyWidgets; 

	bool UICheckReady1 = false;

	bool bIsReadyToReady = false;

	class APCodePlayerController* pc;

	bool onceDo = false;

	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeeReadyButtonUI();

	
};
