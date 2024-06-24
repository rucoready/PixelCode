// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataTypes.h"
#include "BuildingVisual.generated.h"




class ABuilding;
class UMaterialInstance;

UCLASS()
class PIXELCODE_API ABuildingVisual : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingVisual();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = KSH)
	class UStaticMeshComponent* BuildMesh;

	UPROPERTY(EditDefaultsOnly, Category = KSH)
	TSubclassOf< ABuilding> BuildingClass;

// 	UPROPERTY(EditDefaultsOnly, Category = KSH)
// 	TArray<class UStaticMesh*> BuildingMeshes;

	UPROPERTY(EditDefaultsOnly, Category = KSH)
	TArray<FBuildingVisualType> BuildingTypes;

	uint8 BuildingTypeIndex;



	UPROPERTY(EditDefaultsOnly, Category = KSH)
	UMaterialInstance* MaterialFalse;

	UPROPERTY(EditDefaultsOnly, Category = KSH)
	UMaterialInstance* MaterialTrue;

	bool bMaterialIsTrue;



	ABuilding* GetHitBuildingActor(const FHitResult& HitResult);

	ABuilding* InteractingBuilding;



public:	
 
	void SetBuildPosition(const FHitResult& HitResult);
	void SpawnBuilding();
	void CycleMesh();

};
