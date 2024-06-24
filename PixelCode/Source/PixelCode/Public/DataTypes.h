// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/StaticMesh.h"
#include "DataTypes.generated.h"

UENUM(BlueprintType)
enum class EBuildType : uint8
{
	Foundation UMETA(DisplayName = "Foundation"),
	Wall UMETA(DisplayName = "Wall")
};

USTRUCT(BlueprintType)
struct FBuildingVisualType
{
	GENERATED_BODY()

 	UPROPERTY(EditDefaultsOnly, Category = KSH)
 	class UStaticMesh* BuildingMesh;

	UPROPERTY(EditDefaultsOnly, Category = KSH)
	EBuildType BuildType;
};



/**
 * 
 */
//UCLASS()
//class PIXELCODE_API UDataTypes : public UObject
//{
//	GENERATED_BODY()
//	
//};
