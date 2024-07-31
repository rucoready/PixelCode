// Fill out your copyright notice in the Description page of Project Settings.


#include "PCodeSaveGame.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "DataTypes.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "Building.h"


// Fill out your copyright notice in the Description page of Project Settings.
//#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerState.h>

//void APC_Gameplay::BeginPlay()
//{
//    Super::BeginPlay();
//
//    // 플레이어 스테이트가 유효한지 검사하고 유효한 경우에만 Setup_PC 함수를 호출
//    if (ValidatePlayerState())
//    {
//        Setup_PC();
//    }
//}
//void APC_Gameplay::ServerRPC_Spawn_Character_Implementation()
//{
//    UE_LOG(LogTemp, Warning, TEXT("2 APC_Gameplay::ServerRPC_Spawn_Character"));
//    Cast<ASAModeBase>(GetWorld()->GetAuthGameMode())->Spawn_Character(this);
//}
//
//bool APC_Gameplay::ValidatePlayerState()
//{
//    // Ensures that the player state is Valid before we setup the PlayerController
//    // 플레이어 컨트롤러를 설정하기 전에, 플레이어 스테이트가 유효한지 보장한다.
//    FTimerHandle TimerHandle_PlayerStateCheck;
//
//    if (PlayerState->IsValidLowLevel())
//    {
//        UE_LOG(LogTemp, Warning, TEXT("0 APC_Gameplay::PlayerState->IsValid"));
//        return true;
//    }
//    else
//    {
//        UE_LOG(LogTemp, Warning, TEXT("0 APC_Gameplay::PlayerState->Is Not Valid"));
//        GetWorld()->GetTimerManager().SetTimer(TimerHandle_PlayerStateCheck, [&]() {
//            if (ValidatePlayerState())
//            {
//                UE_LOG(LogTemp, Warning, TEXT("0 APC_Gameplay::PlayerState 다시 validate!!!!!!!!"));
//                Setup_PC();
//            }
//            }, 0.2f, false);
//
//        return false;
//    }
//}
//
//void APC_Gameplay::Setup_PC()
//{
//    if (IsLocalPlayerController())
//    {
//        UE_LOG(LogTemp, Warning, TEXT("1 APC_Gameplay::Setup_PC()"));
//        ServerRPC_Spawn_Character();
//    }
//}
//게임 모드에서 Client RPC으로 각자의 세이브 게임에 저장된 캐릭터 커스텀을 불러옴
//void ASAModeBase::Spawn_Character(APlayerController* player)
//{
//    // spawn character 할 때 pawn destroy하고 다시 possess 하는 과정이 있는데 일단 생략하겠음
//    UE_LOG(LogTemp, Warning, TEXT("3 ASAModeBase::Spawn_Character"));
//
//    // player가 null인지 확인
//    if (!player)
//    {
//        UE_LOG(LogTemp, Error, TEXT("Player controller is null"));
//        return;
//    }
//
//    // ATestPlayer로 캐스트가 성공하는지 확인
//    ATestPlayer* TestPlayer = Cast<ATestPlayer>(player->GetPawn());
//    if (!TestPlayer)
//    {
//        UE_LOG(LogTemp, Error, TEXT("Cast to ATestPlayer failed"));
//        return;
//    }
//
//    TestPlayer->ClientRPC_Init_Appearance();
//}
//플레이어 cpp->불러온 캐릭터 커스텀을 실질적으로 입혀 줌
//void ATestPlayer::ClientRPC_Init_Appearance_Implementation()
//{
//    UE_LOG(LogTemp, Warning, TEXT("4 ATestPlayer::ClientRPC_Init_Appearance_Implementation()"));
//    ATestPlayer::Get_Player_Appearance();
//}
//void ATestPlayer::Get_Player_Appearance()
//{
//    UE_LOG(LogTemp, Warning, TEXT("5 ATestPlayer::Get_Player_Appearance()"));
//    Get_Character();
//    Get_Skins();
//    Get_Eyes();
//    Get_Accessory();
//    Get_Top();
//    Get_Bottom();
//    Get_Outer();
//    Get_Dress();
//}
//void ATestPlayer::Get_Character()
//{
//    UE_LOG(LogTemp, Warning, TEXT("6 ATestPlayer::Get_Character()"));
//    FStructure_Player_Appearance_Result result = UFL_General::Get_Player_Appearance();
//    if (result.bSuccess)
//    {
//        ServerRPC_Update_Character(result.Player_Appearance.Character.Mesh);
//    }
//    else
//    {
//        return;
//    }
//}
//void ATestPlayer::ServerRPC_Update_Character_Implementation(USkeletalMesh* _mesh)
//{
//    UE_LOG(LogTemp, Warning, TEXT("7 ATestPlayer::ServerRPC_Update_Character"));
//    Current_SkeletalMesh = _mesh;
//    OnRep_Current_SkeletalMesh();
//}
//void ATestPlayer::OnRep_Current_SkeletalMesh()
//{
//    if (Current_SkeletalMesh->IsValidLowLevelFast())
//    {
//        UE_LOG(LogTemp, Warning, TEXT("8 ATestPlayer::OnRep_Current_SkeletalMesh()"));
//        GetMesh()->SetSkinnedAssetAndUpdate(Current_SkeletalMesh);
//
//        GetMesh()->SetVisibility(true);
//        SM_Outer->SetVisibility(false);
//    }
//}

// void UPCodeSaveGame::AddBuildingData(FBuildingData param)
// {
// 	//.Add(param);
// 	//UGameplayStatics::SaveGameToSlot(this, slotName, 0);
// }
// 
// void UPCodeSaveGame::SaveGame()
// {
//     UPCodeSaveGame* SaveGameInstance = Cast<UPCodeSaveGame>(UGameplayStatics::CreateSaveGameObject(UPCodeSaveGame::StaticClass()));
//     if (SaveGameInstance)
//     {
//         // 월드에서 모든 액터를 가져옴
//         TArray<ABuilding*> ActorsToSave;
//         //UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABuilding::StaticClass(), ActorsToSave);
// 
//         for (TActorIterator<ABuilding> var(GetWorld()); var; ++var)
//         {
//             ActorsToSave.Add( *var);
//         }
// 
//         // 액터들의 정보 저장
//         for (ABuilding* Actor : ActorsToSave)
//         {
//             if (Actor)
//             {
//                 FBuildingData ActorData;
//                 ActorData.ABuilding = Actor->GetClass();
//                 ActorData.BuildingLocation = Actor->GetActorLocation();
//                 ActorData.BuildingRotation = Actor->GetActorRotation();
// 
//                 // 액터 데이터를 세이브 게임 인스턴스에 추가
//                 SaveGameInstance->SavedActors.Add(ActorData);
//             }
//         }
// 
//         // 세이브 게임 슬롯에 저장
//         UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("BuildingDataStorage"), 0);
//     }
//     else
//     {
//         UE_LOG(LogTemp, Warning, TEXT("Failed to create save game instance"));
//     }
// 
// }



