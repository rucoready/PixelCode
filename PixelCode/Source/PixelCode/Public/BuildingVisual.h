// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingVisual.generated.h"



class ABuilding;

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

	UPROPERTY(EditDefaultsOnly, Category = KSH)
	TArray<class UStaticMesh*> BuildingMeshes;

	uint8 BuildingMeshesIndex;


	ABuilding* GetHitBuildingActor(const FHitResult& HitResult);

public:	
 
	void SetBuildPosition(const FHitResult& HitResult);
	void SpawnBuilding();
	void CycleMesh();

};
