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
class UStateComponent;
class APlayerObjectPoolManager;

/**
 * 
 */
UCLASS()
class PIXELCODE_API APCodePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	class APlayerController* PlayerController;

// ������ ---------------------------------------------------------------------

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// �Է� ��� ����, �Ű����� �������� Ŀ�� ǥ��
	void OpenUI(bool bOpen); 


	void SpawnCharacterAtLocation(APixelCodeCharacter* APlayerchar); // 위치에 캐릭터 스폰

	void DeleteCharacter(APixelCodeCharacter* APlayerchar, const FVector& Location); // 위치에 캐릭터 스폰

	void HandleCharacterDeath(); // 캐릭터 사망 처리

	UFUNCTION(Server, Reliable)
	void Server_SpawnAndPossessCharacter(APixelCodeCharacter* CharacterToSpawn);


	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnCharacterAtLocation();

	UFUNCTION(Client, Reliable)
	void ClientRPC_PlayerSpawnWidget();

	/*UFUNCTION(Server, Reliable)
	void Server_SpawnCharacterAtLocation(APixelCodeCharacter* CharacterToSpawn, const FVector& Location);*/

	
	UPROPERTY(Replicated)
	APlayerObjectPoolManager* ObjectPoolManager; // 오브젝트 풀 관리자



public:
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
	
	ApixelPlayerState* pixelPlayerState;

	APixelCodeCharacter* MainPlayer;

	UStateComponent* StatComponent;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// UI

	void FullExp();

	void PlayerLevelUp();

	bool ValidatePlayerState();

	void PlayerWidgetUpdate();

	UFUNCTION(Client, Reliable)
	void ClientRPC_PlayerStartWidget();

	void PlayerBeginWidget();

	void PlayerStatWidget();

	void PlayerStopWidget();

	void PlayerBaseSkillWidget();

	void PlayerSwordSkillWidget();

	void PlayerMageSkillWidget();

	void PlayerDieWidget();

	bool bIsStatVisible = true;

	bool bPlayerState = false;

	//UFUNCTION()
	//void PlayerRespawn();

	//UFUNCTION(Server, Reliable)
	//void ServerRPC_PlayerRespawn();

	//UFUNCTION(Client, Reliable)
	//void ClientRPC_PlayerRespawn();

	/*UFUNCTION(Server, Reliable)
	void ServerRPC_RespawnPlayer();*/

	//UFUNCTION(Server, Reliable)
	//void ServerRPC_ChangeSpectator();
	// 
// ������ �� ---------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UPortalRobbyWidget> portalRobbyWidget;

	class UPortalRobbyWidget* WidgetInstance;

	class AMyGameModeBase* MyGameMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class ULoadingWidget1> loadingWidget1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UBossLoadingWidget> loadingWidgetBoss;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UDamageWidget> damageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UBossEnterWidget> bossEnterWidget;

	class ULoadingWidget1* loadingWidget01;

	class UBossLoadingWidget* bossLoadingWidget;

	class UDamageWidget* damageWidgets;

	class UBossEnterWidget* bossEnterWidgets;

	UFUNCTION(Server, Reliable)
	void ServerRPC_CreateWidgetRobbyWidget();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_CreateWidgetRobbyWidget();

	UFUNCTION(Server, Reliable)
	void ServerRPC_HideWidgetRobbyWidget();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_HideWidgetRobbyWidget();

	UFUNCTION(Server, Reliable)
	void ServerRPC_CreateWidgetLoading1();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_CreateWidgetLoading1();

	UFUNCTION(Server, Reliable)
	void ServerRPC_HideWidgetLoading1();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_HideWidgetLoading1();

	UFUNCTION(Server, Reliable)
	void ServerRPC_CreateWidgetBossLoading();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_CreateWidgetBossLoading();

	UFUNCTION(Server, Reliable)
	void ServerRPC_HideWidgetBossLoading();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_HideWidgetBossLoading();

	UFUNCTION(Server, Reliable)
	void ServerRPC_CreateDamageWidget();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_CreateDamageWidget();

	void ChangeRobbyWidgetButtonReady();

	void CreateWidgetBossEnterWidget();

	UFUNCTION(Server, Reliable)
	void ServerRPC_HideLastBossPortal();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_HideLastBossPortal();

	// 요한

	UFUNCTION(Server, Reliable)
	void ServerRPC_CreateWidgetMyMap();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_CreateWidgetMyMap();

	UFUNCTION(Server, Reliable)
	void ServerRPC_HideWidgetMyMap();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_HideWidgetMyMapLoding();

	void CreateWidgetMyMAPs();

	
	UFUNCTION(Server, Reliable)
	void ServerRPC_HidMyMap();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_HidMyMap();

	

	

	class UMyMapWidget* MyMapEnterWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UMyMapWidget> MyMapEnterWidget;

	class UMyMapLodingWidget* MyMapLoadingWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UMyMapLodingWidget> LoadingWidgetMyMap;

	bool bPoss = false;

};
