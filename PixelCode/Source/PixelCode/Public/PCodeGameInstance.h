// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/DataTable.h"
#include "OnlineSessionSettings.h"
#include "Player/inventory/ItemBase.h"
#include "Player/inventory/InventoryComponent.h"
#include "PCodeGameInstance.generated.h"

/**
 * 
 */

class UDataTable;

 USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString roomName;
	UPROPERTY(BlueprintReadOnly)
	FString hostName;
	UPROPERTY(BlueprintReadOnly)
	FString userName;
	UPROPERTY(BlueprintReadOnly)
	int32 maxPlayerCount;
	UPROPERTY(BlueprintReadOnly)
	int32 currentPlayerCount;
	UPROPERTY(BlueprintReadOnly)
	int32 pingMs;

	int32 index;

	FORCEINLINE void Set(int32 _index, const FOnlineSessionSearchResult& item) {
		index = _index;

		//item.Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
		//item.Session.SessionSettings.Get(FName("HOST_NAME"), hostName);
		// 방장의 이름
		userName = item.Session.OwningUserName;
		// 최대 플레이어 수
		maxPlayerCount = item.Session.SessionSettings.NumPublicConnections;
		// 현재 방에 들어온 플레이어 수
		currentPlayerCount = maxPlayerCount - item.Session.NumOpenPublicConnections;

		pingMs = item.PingInMs;
	}

	FORCEINLINE FString ToString() {
		return FString::Printf(TEXT("%s, %s, %s, (%d/%d), %dms"), *roomName, *hostName, *userName, currentPlayerCount, maxPlayerCount, pingMs);
	}
};


// 방찾기 요청 후 응답이 왔을 때 호출될 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSessioinSearchDelegate, const FSessionInfo&, info);

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSessioinSearchFinishedDelegate, bool, bSearching);

UCLASS()
class PIXELCODE_API UPCodeGameInstance : public UGameInstance
{
	GENERATED_BODY()

	public:
		virtual void Init() override;

		IOnlineSessionPtr sessionInterface;

		FSessioinSearchDelegate OnMySessionSearchCompleteDelegate;

		//FSessioinSearchFinishedDelegate OnMySessioinSearchFinishedDelegate;


		void CreateMySession(FString roomName, int32 PlayerCount);

		//FString mySessionName = TEXT("YoHa");
		FString mySessionName = TEXT("Walpurgis Night ");
	

	UFUNCTION()
	void OnCreateSessionComplete(FName sessionName, bool bWasSuccessful);

	
	void OnFindSessionsComplete(bool bWasSuccressful);

	
	
	TSharedPtr<FOnlineSessionSearch> sessionInSearch;
	void FindOtherSessions();

	
	void JoinMySession(int32 index);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type results);

	FString StringBase64Encode(const FString& str);
	FString StringBase64Decode(const FString& str);

	// -------------------------------------------------- 진원
	UFUNCTION(BlueprintCallable)
	FCharacterStat GetCharacterDataTable(const FString& rowName);

	UFUNCTION(BlueprintCallable)
	FParentItemInfo GetParentItemDataTable(const FString& rowName);
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings")
	UDataTable* dt_characerStatDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings")
	UDataTable* dt_parentItemDataTable;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings")
	UDataTable* dt_CharacterskinDatatable;

	void ExitRoom();

	bool IsInRoom();

	UFUNCTION(Server, Reliable)
	void ServerExitRoom();

	UFUNCTION(NetMulticast, Reliable)
	void MultiExitRoom();

	// -------------------------------------------------- 진원



	//======================================= 요한 SAVE ====================================================

	public:
	//UFUNCTION(BlueprintCallable)
	//void UpdateInventory(const TArray<UItemBase*> InInventory);

	//// 인벤토리 로드
	//TArray<UItemBase*> LoadInventory();

	UFUNCTION(BlueprintCallable)
	//void UpdateInventory(const TArray<UInventoryComponent*> InInventory);
	void UpdateInventory(UInventoryComponent* InInventory);
	//void UpdateInventory(UItemBase* InInventory);

	


	//인벤토리 로드
	TArray<UInventoryComponent*> LoadInventory();

	protected:
	UPROPERTY(EditAnywhere, Category = "KYH")
	TArray<UItemBase*> Inventory;

	UPROPERTY(EditAnywhere, Category = "KYH")
	TArray<UInventoryComponent*> PlayerInventorys;

// 	UPROPERTY(EditAnywhere, Category = KSH)
// 	TArray<ABuilding*> ActorsToSave;


public:
	
// 	FString SavedBuildingDataName = TEXT("BuildingStorage");
// 
// 	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings")
// 	class UPCodeSaveGame* SaveGame;
// 
// 	TSubclassOf<class UPCodeSaveGame> C_SaveGame;
// 
// 	UFUNCTION(BlueprintCallable, Category = KSH)
// 	void AddBuildingData(FBuildingData param);
// 	
// 	UFUNCTION(BlueprintCallable, Category = KSH)
// 	void RemoveBuildingData(FBuildingData param);

};

