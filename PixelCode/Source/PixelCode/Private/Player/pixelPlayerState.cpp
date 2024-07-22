// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/pixelPlayerState.h"
#include "Player/StateComponent.h"
#include "MyGameModeBase.h"
#include "Player/Widget/NormallyWidget.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Player/PlayerStatWidget.h"
#include "PCodePlayerController.h"
#include "Player/PixelCodeCharacter.h"




ApixelPlayerState::ApixelPlayerState()
{
    bReplicates = true;
}

void ApixelPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // totalEXP, currentEXP는 COND_OwnerOnly로 설정
    DOREPLIFETIME_CONDITION(ApixelPlayerState, totalEXP, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(ApixelPlayerState, currentEXP, COND_OwnerOnly);
    // Level은 COND_None으로 설정 (기본값)
    DOREPLIFETIME(ApixelPlayerState, Level);
}

void ApixelPlayerState::SetaddUpEXP(float AcquireEXP)
{
    // 현재 경험치 추가
	currentEXP += AcquireEXP;
    auto* GM = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
    if (GM)
    {
    	// GameMode에 접근하여 원하는 작업 수행
    	GM->EXPmanagement(AcquireEXP, this);
    }
    // 경험치 업데이트 후 최대 경험치 체크
    maxEXP();

    // OnRep 함수 호출
    OnRep_currentEXP(currentEXP);
}

void ApixelPlayerState::LevelUP()
{
    Level += 1;
    maxEXP(); // 레벨 업 시 최대 경험치 업데이트
    //auto* Pc = Cast<APCodePlayerController>(GetPawn()->GetController());
    //if (Pc != nullptr)
    //{
       // Pc->LevelUpUpdate();
   // }

   auto* LevelUpdatePlayer = Cast<APixelCodeCharacter>(GetPawn());
   if (LevelUpdatePlayer != nullptr)
   {
       LevelUpdatePlayer->FullExp();
   }

}

void ApixelPlayerState::InitPlayerData()
{
    Level = 1;
    maxEXP(); // 초기화 시 최대 경험치 설정
}

float ApixelPlayerState::GetCurrentExp() const
{
    

    return currentEXP;
}

int32 ApixelPlayerState::GetCharacterLevel() const
{
    return Level;
}

void ApixelPlayerState::maxEXP()
{
    if (Level == 1)
    {
        totalEXP = 150;
    }
    else if (Level == 2)
    {
        totalEXP = 400;
    }
    else if (Level == 3)
    {
        totalEXP = 650;
    }
    else if (Level == 4)
    {
        totalEXP = 900;
    }
    else if (Level == 5)
    {
        totalEXP = 1150;
    }
}

void ApixelPlayerState::OnRep_totalEXP(int32 OldEXP)
{
    // totalEXP가 변경될 때 실행할 로직 작성
    UE_LOG(LogTemp, Warning, TEXT("Total EXP changed from %d to %d"), OldEXP, totalEXP);
}

void ApixelPlayerState::OnRep_currentEXP(float OldEXP)
{
    // currentEXP가 변경될 때 실행할 로직 작성
	UE_LOG(LogTemp, Warning, TEXT("Current EXP changed from %f to %f"), OldEXP, currentEXP);
}


// 잠시 주석

//ApixelPlayerState::ApixelPlayerState()
//{
//	
//	bReplicates = true;
//
//
//	
//}
//
//
//void ApixelPlayerState::SetaddUpEXP(float AcquireEXP)
//{
//	maxEXP();
//
//	currentEXP += AcquireEXP;
//	
//
//	
//
//
//	//PlayerMainUI->currentExpUpdate(this);
//	//PlayerStatUI->UpdateLevel(this);
//	UE_LOG(LogTemp, Warning, TEXT("UPEXP"));
//
//	OnRep_addUpEXP(AcquireEXP)
//	
//}
//
//void ApixelPlayerState::OnRep_addUpEXP(float AcquireEXP)
//{
//	GM = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
//	if (GM)
//	{
//		// GameMode에 접근하여 원하는 작업 수행
//		GM->EXPmanagement(AcquireEXP, this);
//	}
//}
//
//void ApixelPlayerState::maxEXP()
//{
//	if (Level == 1)
//	{
//		totalEXP = 150;
//	}
//	else if (Level == 2)
//	{
//		totalEXP = 400;
//	}
//	else if (Level == 3)
//	{
//		totalEXP = 650;
//	}
//	else if (Level == 4)
//	{
//		totalEXP = 900;
//	}
//	else if (Level == 5)
//	{
//		totalEXP = 1150;
//	}
//}

//void ApixelPlayerState::LevelUP()
//{
//
//	Level += 1;
//	
//	maxEXP();
//}
//
//
//void ApixelPlayerState::InitPlayerData()
//{
//	//SetPlayerName(TEXT("Destiny")); 이름설정
//	Level = 1; // 나중레벨저장
//
//}
//
//
//
//
//
//int32 ApixelPlayerState::GetCharacterLevel() const
//{
//	
//	return Level;
//}
//
//void ApixelPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	// PlayerName 변수를 Replication 리스트에 추가
//    DOREPLIFETIME_CONDITION(ApixelPlayerState, totalEXP, COND_OwnerOnly);
//	DOREPLIFETIME_CONDITION(ApixelPlayerState, currentEXP, COND_OwnerOnly);
//	DOREPLIFETIME_CONDITION(ApixelPlayerState, Level, COND_None);
//
//}



