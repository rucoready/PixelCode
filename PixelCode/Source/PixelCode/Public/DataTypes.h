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
	Wall UMETA(DisplayName = "Wall"),
	Ceiling UMETA(DisplayName = "Ceiling")
};

USTRUCT(BlueprintType)
struct FBuildingVisualType
{
	GENERATED_BODY()

 	UPROPERTY(EditDefaultsOnly, Category = KSH)
 	class UStaticMesh* BuildingMesh;

	UPROPERTY(EditDefaultsOnly, Category = KSH)
	EBuildType BuildType;

	UPROPERTY(EditDefaultsOnly, Category = KSH)
	FName FilterCharacter;
};

class UInstancedStaticMeshComponent;

USTRUCT(BlueprintType)
struct FBuildingSocketData
{
	GENERATED_BODY()
	
	UInstancedStaticMeshComponent* InstancedComponent;
	int32 Index;
	FName SocketName;
	FTransform SocketTransform;
};

USTRUCT(BlueprintType)
struct FSocketInformation
{
	GENERATED_BODY()

	FString SocketName;
	bool bSocketInUse = false;
};

USTRUCT(BlueprintType)
struct FBuildIndexSockets
{
	GENERATED_BODY()

	int32 Index;
	TArray<FSocketInformation> SocketsInformation;
};

USTRUCT(BlueprintType)
struct FInstanceSocketCheck
{
	GENERATED_BODY()

	UInstancedStaticMeshComponent* InstancedComponent;
	TArray<FBuildIndexSockets> InstanceSocketInformation;
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
