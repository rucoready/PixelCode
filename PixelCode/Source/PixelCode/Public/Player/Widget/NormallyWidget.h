// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NormallyWidget.generated.h"

class UProgressBar;
class UStateComponent;
class ApixelPlayerState;
class APixelCodeCharacter;
class APCodePlayerController;
class UImage;
class UMaterialInstance;
class UMaterialInstanceDynamic;
class UCanvasPanel;
class UButton;
class AMyGameModeBase;
/**
 * 
 */
UCLASS()
class PIXELCODE_API UNormallyWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:

	//virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	UPROPERTY(EditDefaultsOnly, Category = "Stat", meta = (BindWidget))
	UProgressBar* PB_HP;

	UPROPERTY(EditDefaultsOnly, Category = "Stat", meta = (BindWidget))
	UProgressBar* PB_MP;

	UPROPERTY(EditDefaultsOnly, Category = "Stat", meta = (BindWidget))
	UProgressBar* PB_Exp;

	UPROPERTY(EditDefaultsOnly, Category = "Stat")
	UStateComponent* statComp;

	UPROPERTY(EditDefaultsOnly, Category = "GM")
	AMyGameModeBase* GM;

	UPROPERTY(EditDefaultsOnly, Category = "Exp")
	ApixelPlayerState* PlayerState;

	TWeakObjectPtr<class ApixelPlayerState> CurrentPlayerState;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	APixelCodeCharacter* Player;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	APCodePlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "SKillCollTime", meta = (BindWidget))
	UImage* BP_QSkillbar;

	UPROPERTY(EditDefaultsOnly, Category = "SKillCollTime", meta = (BindWidget))
	UImage* BP_ESkillbar;

	UPROPERTY(EditDefaultsOnly, Category = "SKillCollTime", meta = (BindWidget))
	UImage* BP_RSkillbar;

	UPROPERTY(EditDefaultsOnly, Category = "SKillCollTime", meta = (BindWidget))
	UImage* BP_ZSkillbar;

	UPROPERTY(EditDefaultsOnly, Category = "Respawn", meta = (BindWidget))
	UCanvasPanel* CP_GameOverUI;

	UPROPERTY(EditDefaultsOnly, Category = "Respawn", meta = (BindWidget))
	UButton* BTN_Respawn;

	UPROPERTY(EditDefaultsOnly, Category = "Respawn", meta = (BindWidget))
	UButton* BTN_Quit;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material")
	UMaterialInstance* MI_RoundProgressbar;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material")
	UMaterial* BaseMaterial;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material")
	UMaterialInstanceDynamic* QDynamicMaterial;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material")
	UMaterialInstanceDynamic* EDynamicMaterial;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material")
	UMaterialInstanceDynamic* RDynamicMaterial;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material")
	UMaterialInstanceDynamic* ZDynamicMaterial;

	void firstUpdate();
	

	void currentStatUpdate();
	void currentExpUpdate();
	
	void QSetPercent();
	void ESetPercent();
	void RSetPercent();
	void ZSetPercent();

	UFUNCTION()
	void OnMyButtonRespawn();

	UFUNCTION()
	void OnMyButtonQuit();

	void SetActiveGameOverUI(bool value);

};
