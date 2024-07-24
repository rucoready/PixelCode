// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Widget/NormallyWidget.h"
#include "Components/ProgressBar.h"
#include "Player/StateComponent.h"
#include "Player/pixelPlayerState.h"
#include "Player/PixelCodeCharacter.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Materials/MaterialInstance.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Materials/Material.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "PCodePlayerController.h"
#include "PCodeGameInstance.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerState.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Controller.h>





void UNormallyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BaseMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Script/Engine.Material'/Game/Player/PlayerWidget/M_RoundProgressbar.M_RoundProgressbar'_C"));

	
	//DynamicMaterial = LoadObject<UMaterialInstanceDynamic>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Player/PlayerWidgetMI_RoundProgressbar.MI_RoundProgressbar'_C"));

	//UMaterialInstanceDynamic를 생성합니다.
	QDynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	EDynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	RDynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	ZDynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);

	QDynamicMaterial->SetScalarParameterValue(TEXT("Percent"),0.0f);
	EDynamicMaterial->SetScalarParameterValue(TEXT("Percent"),0.0f);
	RDynamicMaterial->SetScalarParameterValue(TEXT("Percent"),0.0f);
	ZDynamicMaterial->SetScalarParameterValue(TEXT("Percent"),0.0f);

	BP_QSkillbar->SetBrushFromMaterial(QDynamicMaterial);
	BP_ESkillbar->SetBrushFromMaterial(EDynamicMaterial);
	BP_RSkillbar->SetBrushFromMaterial(RDynamicMaterial);
	BP_ZSkillbar->SetBrushFromMaterial(ZDynamicMaterial);

	// 리스폰
	BTN_Respawn->OnClicked.AddDynamic(this, &UNormallyWidget::OnMyButtonRespawn);
	BTN_Quit->OnClicked.AddDynamic(this, &UNormallyWidget::OnMyButtonQuit);
	

	
	//
	//AController* Pc = GetOwningPlayerPawn()->GetController();
	//PlayerController = Cast<APCodePlayerController>(Pc);

	//if (PlayerController)
	//{
	//	PlayerState = PlayerController->GetPlayerStateOfOtherPlayer();
	//	if (PlayerState != nullptr)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("NormallyOnPlayerState"));

	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("NormallyNotPlayerState"));
	//	
	//	}
	//	UE_LOG(LogTemp, Warning, TEXT("Pc"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("NotPc"));
	//}


	Player = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());

	if (Player != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Player Null!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Null!"));
	}
	//PlayerController = Cast<APCodePlayerController>(Player->GetController());
	//PlayerState = Cast<ApixelPlayerState>(PlayerController->pixelPlayerState);


	//PlayerState = Cast<ApixelPlayerState>(Player->GetPlayerState());

	//ApixelPlayerState* CustomPlayerState = Cast<ApixelPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(),0));
	

	
	//currentLevelUpdate();
	//firstUpdate();
	//currentExpUpdate(PlayerState->currentEXP, PlayerState->totalEXP);
	//firstStatedate();
}

void UNormallyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);

	//currentExpUpdate(PlayerState->currentEXP, PlayerState->totalEXP);

	QSetPercent();
	ESetPercent();
	RSetPercent();
	ZSetPercent();
}


void UNormallyWidget::firstUpdate(UStateComponent* PlayerStateComp)
{
	PB_HP->SetPercent(PlayerStateComp->MaxHP);
	PB_MP->SetPercent(PlayerStateComp->MaxMP);
	//UE_LOG(LogTemp, Warning, TEXT("PlayerStateNonull"));
}
void UNormallyWidget::firstStatedate(ApixelPlayerState* Ps)
{
	PB_Exp->SetPercent(PlayerState->currentEXP);

	LEVEL = FString::FromInt(PlayerState->Level);  // float을 FString으로 변환
	TB_LEVEL->SetText(FText::FromString(LEVEL));  // FString을 FText로 변환하여 UTextBlock에 설정
}


void UNormallyWidget::currentStatUpdate(UStateComponent* PlayerStateComp)
{
		PB_HP->SetPercent(PlayerStateComp->currentHP/PlayerStateComp->MaxHP);
		PB_MP->SetPercent(PlayerStateComp->MaxMP/PlayerStateComp->currentMP);
}

void UNormallyWidget::currentExpUpdate(float currentEXP,float totalEXP)
{
	PB_Exp->SetPercent(currentEXP/totalEXP);
	//UE_LOG(LogTemp, Warning, TEXT("UPdateEXP"));
}

void UNormallyWidget::currentLevelUpdate(ApixelPlayerState* Ps)
{
	LEVEL = FString::FromInt(PlayerState->Level);
	TB_LEVEL->SetText(FText::FromString(LEVEL));  
}

void UNormallyWidget::QSetPercent()
{
	if (QDynamicMaterial)
	{
		if (Player!= nullptr &&Player->CurrentQSkillCoolTime != 0)
		{
			// 스칼라 파라미터 설정
			QDynamicMaterial->SetScalarParameterValue(TEXT("Percent"), 1.0f - Player->CurrentQSkillCoolTime/Player->QSkillCoolTime);
		}

		// BP_QSkillbar가 UImage인 경우 SetBrushFromMaterial을 사용할 수 있습니다.
		BP_QSkillbar->SetBrushFromMaterial(QDynamicMaterial);
	}
}

void UNormallyWidget::ESetPercent()
{
	if (QDynamicMaterial)
	{
		if (Player != nullptr && Player->CurrentESkillCoolTime != 0)
		{
			// 스칼라 파라미터 설정
			float PercentValue = 1.0f - Player->CurrentESkillCoolTime / Player->ESkillCoolTime;
			EDynamicMaterial->SetScalarParameterValue(TEXT("Percent"), 1.0f - Player->CurrentESkillCoolTime /Player->ESkillCoolTime);
			UE_LOG(LogTemp, Log, TEXT("Percent set to: %f"), PercentValue);
		}

		BP_ESkillbar->SetBrushFromMaterial(EDynamicMaterial);
	}
}

void UNormallyWidget::RSetPercent()
{
	if (RDynamicMaterial)
	{
		if (Player != nullptr && Player->CurrentRSkillCoolTime != 0)
		{
			// 스칼라 파라미터 설정
			RDynamicMaterial->SetScalarParameterValue(TEXT("Percent"), 1.0f - Player->CurrentRSkillCoolTime / Player->RSkillCoolTime);
		}

	
		BP_RSkillbar->SetBrushFromMaterial(RDynamicMaterial);
	}
}

void UNormallyWidget::ZSetPercent()
{
	if (ZDynamicMaterial)
	{
		if (Player != nullptr && Player->CurrentZSkillCoolTime != 0)
		{
			// 스칼라 파라미터 설정
			ZDynamicMaterial->SetScalarParameterValue(TEXT("Percent"), 1.0f - Player->CurrentZSkillCoolTime / Player->ZSkillCoolTime);
		}

		
		BP_ZSkillbar->SetBrushFromMaterial(ZDynamicMaterial);
	}
}

void UNormallyWidget::OnMyButtonRespawn()
{
	// 게임오버UI를 보이지않게하고
	SetActiveGameOverUI(false);
	auto* pc = Cast<APCodePlayerController>(GetWorld()->GetFirstPlayerController());
	if (pc)
	{
		// 플레이어컨트롤러를 통해 재시작하고싶다.
		pc->SetInputMode(FInputModeGameOnly());
		pc->SetShowMouseCursor(false);
		pc->ServerRPC_RespawnPlayer();
		//pc->ServerRPC_ChangeSpectator();
	}
}

void UNormallyWidget::OnMyButtonQuit()
{
	//UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);

	auto* gi = Cast<UPCodeGameInstance>(GetWorld()->GetGameInstance());
	if (gi)
	{
		gi->ExitRoom();
	}
}

void UNormallyWidget::SetActiveGameOverUI(bool value)
{
	CP_GameOverUI ->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}



