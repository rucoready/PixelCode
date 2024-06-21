// Fill out your copyright notice in the Description page of Project Settings.


#include "PCodeGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include <../../../../../../../Source/Runtime/Core/Public/Templates/SharedPointer.h>
#include <string>

void UPCodeGameInstance::Init()
{
	Super::Init();

	auto subsys = IOnlineSubsystem::Get();
	if(subsys)
	{
		sessionInterface = subsys->GetSessionInterface();

		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPCodeGameInstance::OnCreateSessionComplete);
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPCodeGameInstance::OnFindSessionsComplete);
		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPCodeGameInstance::OnJoinSessionComplete);
	}
}

void UPCodeGameInstance::CreateMySession(FString roomName, int32 PlayerCount)
{
	FOnlineSessionSettings set;

	set.bIsDedicated = false;
	
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	set.bIsLANMatch = subsysName =="NULL";

	// 매칭 방을 공개할건인가 YES
	set.bShouldAdvertise = true;

	//Presence 를 사용할건인가 ? yes
	set.bUsesPresence = true;

	// 게임 도중 참여 가능 여부
	set.bAllowJoinInProgress = true;
	set.bAllowJoinViaPresence = true;

	set.NumPublicConnections = PlayerCount;

	// 커스텀 정보 : 방 이름 , 호스트 이름
	set.Set(FName("ROOM_NAME"), roomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	set.Set(FName("HOST_NAME"), mySessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// NET ID 가져오기
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	sessionInterface->CreateSession(*netID, FName(*roomName), set);

}


void UPCodeGameInstance::FindOtherSessions()
{
	sessionInSearch = MakeShareable(new FOnlineSessionSearch);
	

	// 검색 조건 설정
	sessionInSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	sessionInSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";

	sessionInterface->FindSessions(0, sessionInSearch.ToSharedRef());

	// 최대 검색 수 
	sessionInSearch->MaxSearchResults = 30;

}

void UPCodeGameInstance::JoinMySession(int32 index)
{
	sessionInterface->JoinSession(0, FName(*mySessionName), sessionInSearch->SearchResults[index]);
}

void UPCodeGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	// 호스트의 IP, Port번호를 가져와서 ClientTravel 하고싶다.
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		auto* pc = GetWorld()->GetFirstPlayerController();
		FString url;
		sessionInterface->GetResolvedConnectString(SessionName, url);
		UE_LOG(LogTemp, Warning, TEXT("ClientTravel URL : %s"), *url);

		if (false == url.IsEmpty())
		{
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
		}
	}
}


void UPCodeGameInstance::OnCreateSessionComplete(FName sessionName, bool bWasSuccessful)
{
	// 만약 방생성을 성공했다면 ServerTravel 하고싶다.
	if (bWasSuccessful)
	{
		GetWorld()->ServerTravel(TEXT("/Game/MainMap/MainMap?listen"));
	}
}

void UPCodeGameInstance::OnFindSessionsComplete(bool bWasSuccressful)
{
	// 만약 성공했다면
		if (bWasSuccressful)
		{
			// sessioinSearch에서 정보를 가져오고싶다. -> UI로 표현하고싶다.
			auto results = sessionInSearch->SearchResults;
			if (results.Num() == 0) {
				UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete results.Num() == 0"));
			}
			for (int i = 0; i < results.Num(); i++)
			{
				auto item = results[i];
				if (false == item.IsValid()) { continue; }

				FSessionInfo info;
				info.Set(i, item);

				FString roomName;
				FString hostName;
				item.Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
				item.Session.SessionSettings.Get(FName("HOST_NAME"), hostName);

				info.roomName = StringBase64Decode(roomName);
				info.hostName = StringBase64Decode(hostName);


				OnMySessionSearchCompleteDelegate.Broadcast(info);
				UE_LOG(LogTemp, Warning, TEXT("%s"), *info.ToString());
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete bWasSuccessful is false"));
			// UI의 Finding...을 활성화 해달라...
		}

	if (OnMySessioinSearchFinishedDelegate.IsBound())
	{
		OnMySessioinSearchFinishedDelegate.Broadcast(false);
	}
}


FString UPCodeGameInstance::StringBase64Encode(const FString& str)
{
	// Set 할 때 : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 로 Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()), utf8String.length());
	return FBase64::Encode(arrayData);
}

FString UPCodeGameInstance::StringBase64Decode(const FString& str)
{
	// Get 할 때 : base64 로 Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(str, arrayData);
	std::string ut8String((char*)(arrayData.GetData()), arrayData.Num());
	return UTF8_TO_TCHAR(ut8String.c_str());
}
