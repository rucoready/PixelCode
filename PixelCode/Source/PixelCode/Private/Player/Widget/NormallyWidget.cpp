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
#include "PCodePlayerController.h"
#include "PCodeGameInstance.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"





void UNormallyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = nullptr;

	// GetOwningPlayer() �Լ��� ����Ͽ� Widget�� ���� Player�� �����ɴϴ�.
	auto* OwningPlayer = GetOwningPlayer();
	if (OwningPlayer)
	{
		// OwningPlayer���� MyCharacter�� �ٿ�ĳ�����մϴ�.
		Player = Cast<APixelCodeCharacter>(OwningPlayer->GetPawn());
	}

	BaseMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Script/Engine.Material'/Game/Player/PlayerWidget/M_RoundProgressbar.M_RoundProgressbar'_C"));

	
	//DynamicMaterial = LoadObject<UMaterialInstanceDynamic>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Player/PlayerWidgetMI_RoundProgressbar.MI_RoundProgressbar'_C"));

	//UMaterialInstanceDynamic�� �����մϴ�.
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

	// ������
	BTN_Respawn->OnClicked.AddDynamic(this, &UNormallyWidget::OnMyButtonRespawn);
	BTN_Quit->OnClicked.AddDynamic(this, &UNormallyWidget::OnMyButtonQuit);
	
	firstUpdate();

}

void UNormallyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);


	QSetPercent();
	ESetPercent();
	RSetPercent();
	ZSetPercent();
}



void UNormallyWidget::firstUpdate()
{
	
	PB_HP->SetPercent(Player->stateComp->MaxHP);
	PB_MP->SetPercent(Player->stateComp->MaxMP);
	UE_LOG(LogTemp, Warning, TEXT("PlayerStateNonull"));
	//PB_Exp->SetPercent(playerState->totalEXP);
	
	/*if (Player->stateComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerStatenull"));
	}*/

}


void UNormallyWidget::currentStatUpdate()
{
	PB_HP->SetPercent(Player->stateComp->currentHP/Player->stateComp->MaxHP);
	PB_MP->SetPercent(Player->stateComp->MaxMP/Player->stateComp->currentMP);
}

void UNormallyWidget::currentExpUpdate()
{
	PB_Exp->SetPercent(playerState->currentEXP/ playerState->totalEXP);
}

void UNormallyWidget::QSetPercent()
{
	if (QDynamicMaterial)
	{
		if (Player!= nullptr &&Player->CurrentQSkillCoolTime != 0)
		{
			// ��Į�� �Ķ���� ����
			QDynamicMaterial->SetScalarParameterValue(TEXT("Percent"), 1.0f - Player->CurrentQSkillCoolTime/Player->QSkillCoolTime);
		}

		// BP_QSkillbar�� UImage�� ��� SetBrushFromMaterial�� ����� �� �ֽ��ϴ�.
		BP_QSkillbar->SetBrushFromMaterial(QDynamicMaterial);
	}
}

void UNormallyWidget::ESetPercent()
{
	if (QDynamicMaterial)
	{
		if (Player != nullptr && Player->CurrentESkillCoolTime != 0)
		{
			// ��Į�� �Ķ���� ����
			EDynamicMaterial->SetScalarParameterValue(TEXT("Percent"), 1.0f - Player->CurrentESkillCoolTime / Player->ESkillCoolTime);
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
			// ��Į�� �Ķ���� ����
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
			// ��Į�� �Ķ���� ����
			ZDynamicMaterial->SetScalarParameterValue(TEXT("Percent"), 1.0f - Player->CurrentZSkillCoolTime / Player->ZSkillCoolTime);
		}

		
		BP_ZSkillbar->SetBrushFromMaterial(ZDynamicMaterial);
	}
}

void UNormallyWidget::OnMyButtonRespawn()
{
	// ���ӿ���UI�� �������ʰ��ϰ�
	SetActiveGameOverUI(false);
	auto* pc = Cast<APCodePlayerController>(GetWorld()->GetFirstPlayerController());
	if (pc)
	{
		// �÷��̾���Ʈ�ѷ��� ���� ������ϰ�ʹ�.
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



