// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PCodePlayerController.generated.h"


class AMyGameModeBase;
class UNormallyWidget;
class UPlayerStatWidget;
class ApixelPlayerState;
class APixelCodeCharacter;
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

	UPROPERTY(Replicated)
	UPlayerStatWidget* statWidget;
	
	

	UPROPERTY(Replicated)
	UNormallyWidget* NormallyWidget;
	
	ApixelPlayerState* PlayerState;

	APixelCodeCharacter* MainPlayer;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// UI
	

	//UFUNCTION(Server, Reliable)
	//void ServerRPC_StartUI();

	//UFUNCTION(Client, Reliable)
	//void ClientRPC_StartUI();

	bool bIsStatVisible = true;


	UFUNCTION(Server, Reliable)
	void ServerRPC_RespawnPlayer();

	
	UFUNCTION(Server, Reliable)
	void ServerSendPlayerStateToClient(APlayerController* TargetPlayerController, ApixelPlayerState* PlayerStateData);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastReceivePlayerStateFromServer(APlayerController* TargetPlayerController, ApixelPlayerState* PlayerStateData);

protected:




	//UFUNCTION(Server, Reliable)
	//void ServerRPC_ChangeSpectator();
// ������ �� ---------------------------------------------------------------------


};
