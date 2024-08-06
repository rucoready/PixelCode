// Fill out your copyright notice in the Description page of Project Settings.


#include "PCodeGameInstance.h"
#include "OnlineSubsystem.h"
#include "Player/StateComponent.h"
#include "Player/ParentItem.h"
#include <../../../../../../../Source/Runtime/Core/Public/Templates/SharedPointer.h>
#include "OnlineSessionSettings.h"
#include "PCodeSaveGame.h"

#include "Online/OnlineSessionNames.h"
//#include <../../../../../../../Source/Runtime/Core/Public/Templates/SharedPointer.h>
#include <xstring>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetArrayLibrary.h>

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

 	// 이미 세이브 된 빌딩 데이터가 있는지 확인해서 로드하기
// 	if (UGameplayStatics::DoesSaveGameExist("BuildingDataStorage", 0))
//  	{
// 		UE_LOG(LogTemp, Warning, TEXT("-----------------------------*&^(*&^(*&^(*&^(*&^(*&^(*&^*(&^(*&^(*&^(^ Casted :: Ready to Load"));
// 		//UPCodeSaveGame*saveGameCast = Cast<UPCodeSaveGame>(UGameplayStatics::LoadGameFromSlot("BuildingDataStorage", 0));
// 		
// 		//saveGameCast = Cast<UPCodeSaveGame>(UGameplayStatics::LoadGameFromSlot("BuildingDataStorage", 0));
// 		UE_LOG(LogTemp, Warning, TEXT("-----------------------------*&^(*&^(*&^(*&^(*&^(*&^(*&^*(&^(*&^(*&^(^ Load Successful"));
// 
//  	}
//  	else
//  	{
// 		UE_LOG(LogTemp, Warning, TEXT("-----------------------------*&^(*&^(*&^(*&^(*&^(*&^(*&^*(&^(*&^(*&^(^ No Save Game EXIST :: Ready to Create"));
// 
// 		UGameplayStatics::CreateSaveGameObject(UPCodeSaveGame::StaticClass());
// 		UE_LOG(LogTemp, Warning, TEXT("-----------------------------*&^(*&^(*&^(*&^(*&^(*&^(*&^*(&^(*&^(*&^(^ No Save Game EXIST :: CreateSaveGameObject"));
// 
//  	}
}

void UPCodeGameInstance::CreateMySession(FString roomName,  int32 PlayerCount)
{
	FOnlineSessionSettings set;

	set.bIsDedicated = false;
	
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	set.bIsLANMatch = subsysName == "NULL";

	// 매칭 방을 공개할건인가 YES
	set.bShouldAdvertise = true;

	//Presence 를 사용할건인가 ? yes
	set.bUsesPresence = true;

	// 게임 도중 참여 가능 여부
	set.bAllowJoinInProgress = true;
	set.bAllowJoinViaPresence = true;

	set.NumPublicConnections = PlayerCount;

	// 6. 참여할 공개 연결의 최대 갯수
	//UE_LOG(LogTemp, Warning, TEXT("set.NumPublicConnections: %d,"), set.NumPublicConnections);
	PlayerCount = (PlayerCount == 0) ? 2 : PlayerCount;

	if (PlayerCount == 0)
	{
		PlayerCount = 10;
	}

	set.NumPublicConnections = PlayerCount;

	//UE_LOG(LogTemp, Warning, TEXT("set.NumPublicConnections: %d,"), set.NumPublicConnections);

	// 커스텀 정보 : 방 이름 , 호스트 이름
	//set.Set(FName("ROOM_NAME"), roomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	//set.Set(FName("HOST_NAME"), mySessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	

	set.Set(FName("ROOM_NAME"), StringBase64Encode(roomName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	set.Set(FName("HOST_NAME"), StringBase64Encode(mySessionName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// NET ID 가져오기
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	sessionInterface->CreateSession(*netID, FName(*roomName), set);

}


void UPCodeGameInstance::FindOtherSessions()
{
	//sessionInSearch = MakeShareable(new FOnlineSessionSearch);
	//

	//// 검색 조건 설정
	//sessionInSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	//sessionInSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";

	//sessionInterface->FindSessions(0, sessionInSearch.ToSharedRef());

	//// 최대 검색 수 
	//sessionInSearch->MaxSearchResults = 30
	// 세션 인터페이스를 이용해서 방을 찾고 싶다. 

	sessionInSearch = MakeShareable(new FOnlineSessionSearch);
	//UE_LOG(LogTemp, Warning, TEXT("11111111111111"));

	// 검색 조건을 설정 하고싶다.

	sessionInSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Near);
	//UE_LOG(LogTemp, Warning, TEXT("2222222222"));

	// LAN 여부
	sessionInSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	//UE_LOG(LogTemp, Warning, TEXT("33333333333"));

	// 최대 검색 수
	sessionInSearch->MaxSearchResults = 30;
	//UE_LOG(LogTemp, Warning, TEXT("44444444444444444"));

	sessionInterface->FindSessions(0, sessionInSearch.ToSharedRef());


}

void UPCodeGameInstance::OnCreateSessionComplete(FName sessionName, bool bWasSuccessful)
{
	// 만약 방생성을 성공했다면 ServerTravel 하고싶다.
	if (bWasSuccessful)
	{
		GetWorld()->ServerTravel(TEXT("/Game/KSH/Maps/MAP?listen"));
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
				//UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete results.Num() == 0"));
				//UE_LOG(LogTemp, Warning, TEXT(" i kill you"));
			
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

				//UE_LOG(LogTemp, Warning, TEXT("%s"), *info.ToString());
			}
		
		}

		/*if (OnMySessioinSearchFinishedDelegate.IsBound())
		{
			OnMySessioinSearchFinishedDelegate.Broadcast(false);
		}*/
}

void UPCodeGameInstance::JoinMySession(int32 index)
{
	sessionInterface->JoinSession(0, FName(*mySessionName), sessionInSearch->SearchResults[index]);
	//UE_LOG(LogTemp, Warning, TEXT("777777777"));
}

void UPCodeGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type results)
{
	// 호스트의 IP, Port번호를 가져와서 ClientTravel 하고싶다.
	if (results == EOnJoinSessionCompleteResult::Success)
	{
		auto* pc = GetWorld()->GetFirstPlayerController();
		FString url;
		sessionInterface->GetResolvedConnectString(SessionName, url);
		//UE_LOG(LogTemp, Warning, TEXT("ClientTravel URL : %s"), *url);

		if (false == url.IsEmpty())
		{
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
		}
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

// 진원 ----------------------------------------------------------------------

FCharacterStat UPCodeGameInstance::GetCharacterDataTable(const FString& rowName)
{
	if (dt_characerStatDataTable != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("gameins"));
		FString errorText;
		FCharacterStat* characerStatInfo = dt_characerStatDataTable->FindRow<FCharacterStat>(FName(rowName), errorText);

		if (characerStatInfo != nullptr)
		{
			return *characerStatInfo;
		}
		else
		{
			return FCharacterStat();
		}
	}

	return FCharacterStat();
}

FParentItemInfo UPCodeGameInstance::GetParentItemDataTable(const FString& rowName)
{
	if (dt_parentItemDataTable != nullptr)
	{
		FString errorText;
		FParentItemInfo* parentItemInfo = dt_parentItemDataTable->FindRow<FParentItemInfo>(FName(rowName), errorText);

		if (parentItemInfo != nullptr)
		{
			return *parentItemInfo;
		}
		else
		{
			return FParentItemInfo();
		}
	}

	return FParentItemInfo();
}

void UPCodeGameInstance::ExitRoom()
{
	ServerExitRoom();
}

void UPCodeGameInstance::ServerExitRoom_Implementation()
{
	MultiExitRoom();
}

void UPCodeGameInstance::MultiExitRoom_Implementation()
{
	sessionInterface->DestroySession(FName(*mySessionName));
}

// 진원 끝--------------------------------------------------------------------

void UPCodeGameInstance::UpdateInventory(UInventoryComponent* InInventory)
{
	InInventory->GetInventoryContents();
}


TArray<UInventoryComponent*> UPCodeGameInstance::LoadInventory()
{
	return PlayerInventorys;
}


// 서휘---------------빌딩 세이브
// 	void UPCodeGameInstance::AddBuildingData(FBuildingData param)
// 	{
// 		SaveGame->Arr_BuildingData.Add(param);
// 	
// 		UGameplayStatics::SaveGameToSlot(SaveGame, SavedBuildingDataName, 0);
// 	}
// 	
// 	void UPCodeGameInstance::RemoveBuildingData(FBuildingData param)
// 	{
// 		for (FBuildingData* data : SaveGame->Arr_BuildingData)
// 		{
// 	
// 		}
// 		UGameplayStatics::SaveGameToSlot(SaveGame, SavedBuildingDataName, 0);
//}