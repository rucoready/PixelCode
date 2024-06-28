// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "PCodeGameInstance.generated.h"

/**
 * 
 */

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

		FString mySessionName = TEXT("PixelCode");
	

	UFUNCTION()
	void OnCreateSessionComplete(FName sessionName, bool bWasSuccessful);

	
	void OnFindSessionsComplete(bool bWasSuccressful);

	
	
	TSharedPtr<FOnlineSessionSearch> sessionInSearch;
	void FindOtherSessions();

	
	void JoinMySession(int32 index);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type results);

	FString StringBase64Encode(const FString& str);
	FString StringBase64Decode(const FString& str);

};
