// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UMainMenuWidget; // 뭐가 사용됐는지 알아보기위해, 헤더따로 추가 안해도 됨.
class UInteractionWidget;
class UPlayerStatWidget;
//class ULootPanel;
class UCraftingWidget;
class UBulidWIdget;
struct FInteractableData;

// HUD는 위젯을 생성하기 완벽한 장소
/**
 *
 */

/**
 * 
 */
UCLASS()
class PIXELCODE_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	

public:
	//================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================
	 
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;



	bool bIsMenuVisible; // 조준모드에 있는 다음메뉴 전환X하기위함 불값

	//================================================================================
	// FUNCTIONS
	//================================================================================
	APlayerHUD();


	void DisplayMenu(); // 표시메뉴기능
	void HideMenu(); // 메뉴숨기기
	void ToggleMenu();


	void ShowInteractionWidget()const;
	void HideInteractionWidget()const;
	void UpdateInteractionWidget(const FInteractableData& InteractableData)const; // 상호작용가능데이터

	

	//요한 ------------------------------====================================================================
	void ShowOrHideCrafting();
	void HideCrafting();
	bool bIsCreatVisible;
	void ToggleCreate();
	void creatwidgets();

	 
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UCraftingWidget> CraftingClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	UCraftingWidget* Crafting;


	void ShowBuilding();
	void HideBuildinging();
	bool bIsbuildVisible;
	void ToggleBuilding();
	void Buildwidgets();

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	UBulidWIdget* Building;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UBulidWIdget> BuildingClass;
	
	

protected:

	//================================================================================
	// PROPERTIES & VARIABLES
	UPROPERTY()
	UMainMenuWidget* MainManuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;


	//================================================================================

	//================================================================================
	// FUNCTIONS
	//================================================================================
	virtual void BeginPlay() override;

};
