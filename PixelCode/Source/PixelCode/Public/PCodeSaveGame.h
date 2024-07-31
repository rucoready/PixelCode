// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DataTypes.h"
#include "PCodeSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UPCodeSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	
	FString slotName = "BuildingDataStorage";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KSH)
	TArray<FBuildingActorData> SavedActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KSH)
	TArray<FBuildingInstanceData> SavedInstances;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KSH)
// 	UPCodeSaveGame* saveGameCast;

};
