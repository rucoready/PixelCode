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

	// ��Ī ���� �����Ұ��ΰ� YES
	set.bShouldAdvertise = true;

	//Presence �� ����Ұ��ΰ� ? yes
	set.bUsesPresence = true;

	// ���� ���� ���� ���� ����
	set.bAllowJoinInProgress = true;
	set.bAllowJoinViaPresence = true;

	set.NumPublicConnections = PlayerCount;

	// Ŀ���� ���� : �� �̸� , ȣ��Ʈ �̸�
	set.Set(FName("ROOM_NAME"), roomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	set.Set(FName("HOST_NAME"), mySessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// NET ID ��������
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	sessionInterface->CreateSession(*netID, FName(*roomName), set);

}


void UPCodeGameInstance::FindOtherSessions()
{
	sessionInSearch = MakeShareable(new FOnlineSessionSearch);
	

	// �˻� ���� ����
	sessionInSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	sessionInSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";

	sessionInterface->FindSessions(0, sessionInSearch.ToSharedRef());

	// �ִ� �˻� �� 
	sessionInSearch->MaxSearchResults = 30;

}

void UPCodeGameInstance::JoinMySession(int32 index)
{
	sessionInterface->JoinSession(0, FName(*mySessionName), sessionInSearch->SearchResults[index]);
}

void UPCodeGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	// ȣ��Ʈ�� IP, Port��ȣ�� �����ͼ� ClientTravel �ϰ�ʹ�.
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
	// ���� ������� �����ߴٸ� ServerTravel �ϰ�ʹ�.
	if (bWasSuccessful)
	{
		GetWorld()->ServerTravel(TEXT("/Game/MainMap/MainMap?listen"));
	}
}

void UPCodeGameInstance::OnFindSessionsComplete(bool bWasSuccressful)
{
	// ���� �����ߴٸ�
		if (bWasSuccressful)
		{
			// sessioinSearch���� ������ ��������ʹ�. -> UI�� ǥ���ϰ�ʹ�.
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
			// UI�� Finding...�� Ȱ��ȭ �ش޶�...
		}

	if (OnMySessioinSearchFinishedDelegate.IsBound())
	{
		OnMySessioinSearchFinishedDelegate.Broadcast(false);
	}
}


FString UPCodeGameInstance::StringBase64Encode(const FString& str)
{
	// Set �� �� : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 �� Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()), utf8String.length());
	return FBase64::Encode(arrayData);
}

FString UPCodeGameInstance::StringBase64Decode(const FString& str)
{
	// Get �� �� : base64 �� Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(str, arrayData);
	std::string ut8String((char*)(arrayData.GetData()), arrayData.Num());
	return UTF8_TO_TCHAR(ut8String.c_str());
}
