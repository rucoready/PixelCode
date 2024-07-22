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

    // totalEXP, currentEXP�� COND_OwnerOnly�� ����
    DOREPLIFETIME_CONDITION(ApixelPlayerState, totalEXP, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(ApixelPlayerState, currentEXP, COND_OwnerOnly);
    // Level�� COND_None���� ���� (�⺻��)
    DOREPLIFETIME(ApixelPlayerState, Level);
}

void ApixelPlayerState::SetaddUpEXP(float AcquireEXP)
{
    // ���� ����ġ �߰�
	currentEXP += AcquireEXP;
    auto* GM = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
    if (GM)
    {
    	// GameMode�� �����Ͽ� ���ϴ� �۾� ����
    	GM->EXPmanagement(AcquireEXP, this);
    }
    // ����ġ ������Ʈ �� �ִ� ����ġ üũ
    maxEXP();

    // OnRep �Լ� ȣ��
    OnRep_currentEXP(currentEXP);
}

void ApixelPlayerState::LevelUP()
{
    Level += 1;
    maxEXP(); // ���� �� �� �ִ� ����ġ ������Ʈ
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
    maxEXP(); // �ʱ�ȭ �� �ִ� ����ġ ����
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
    // totalEXP�� ����� �� ������ ���� �ۼ�
    UE_LOG(LogTemp, Warning, TEXT("Total EXP changed from %d to %d"), OldEXP, totalEXP);
}

void ApixelPlayerState::OnRep_currentEXP(float OldEXP)
{
    // currentEXP�� ����� �� ������ ���� �ۼ�
	UE_LOG(LogTemp, Warning, TEXT("Current EXP changed from %f to %f"), OldEXP, currentEXP);
}


// ��� �ּ�

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
//		// GameMode�� �����Ͽ� ���ϴ� �۾� ����
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
//	//SetPlayerName(TEXT("Destiny")); �̸�����
//	Level = 1; // ���߷�������
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
//	// PlayerName ������ Replication ����Ʈ�� �߰�
//    DOREPLIFETIME_CONDITION(ApixelPlayerState, totalEXP, COND_OwnerOnly);
//	DOREPLIFETIME_CONDITION(ApixelPlayerState, currentEXP, COND_OwnerOnly);
//	DOREPLIFETIME_CONDITION(ApixelPlayerState, Level, COND_None);
//
//}



