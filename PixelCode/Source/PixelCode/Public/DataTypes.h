// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/StaticMesh.h"
#include "DataTypes.generated.h"

UENUM(BlueprintType)
enum class EBuildType : uint8
{
	Base UMETA(DisplayName = "Base"),
	Wall UMETA(DisplayName = "Wall"),
	Ceiling UMETA(DisplayName = "Ceiling"),
	Floor UMETA(DisplayName = "Floor"),
	Window UMETA(DisplayName = "Window"),
	Arch UMETA(DisplayName = "Arch"),
	Stairs UMETA(DisplayName = "Stairs"),
	Roof UMETA(DisplayName = "Roof"),
	Gable UMETA(DisplayName = "Gable")	
};

USTRUCT(BlueprintType)
struct FBuildingVisualType
{
	GENERATED_BODY()

 	UPROPERTY(EditDefaultsOnly, Category = KSH)
 	class UStaticMesh* BuildingMesh;

	UPROPERTY( EditDefaultsOnly, Category = KSH)
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

USTRUCT(BlueprintType)
struct FBuildingActorData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KSH")
    TSubclassOf<class ABuilding> ABuilding;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KSH")
    FVector BuildingLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KSH")
	FRotator BuildingRotation;

};

USTRUCT(BlueprintType)
struct FBuildingInstanceData
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KSH")
	UInstancedStaticMeshComponent* InstancedComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KSH")
	FTransform InstTransform;	  
};


//UCLASS()
//class PIXELCODE_API UDataTypes : public UObject
//{
//	GENERATED_BODY()
//	
//};
