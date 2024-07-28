// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTypes.h"
#include "BulidWIdget.generated.h"



class AItemStorage;
class UScrollBox;
class APixelCodeCharacter;
class UTextBlock;
class UImage;
class UHorizontalBox;
class UButton;
class USoundBase;
class UBuildItemSlot;
/**
 * 
 */
UCLASS()
class PIXELCODE_API UBulidWIdget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;



protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_BuildVisual;

	UPROPERTY(meta = (BindWidget))
	UImage* I_BuildVIsual;


	UPROPERTY(EditAnywhere, Category = "CRAFT")
	APixelCodeCharacter* Char;

	UPROPERTY(VisibleAnywhere, Category = "CRAFT")
	TArray<TWeakObjectPtr<UBuildItemSlot>> BuildItems;

};
