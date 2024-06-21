// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingVisual.h"
#include "Building.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ABuildingVisual::ABuildingVisual()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	BuildMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildMeshComponent"));
	RootComponent = BuildMesh;

	BuildingMeshesIndex = 0;
}

// Called when the game starts or when spawned
void ABuildingVisual::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorHiddenInGame(true);

	if (BuildingMeshes[BuildingMeshesIndex])
	{
		BuildMesh->SetStaticMesh(BuildingMeshes[BuildingMeshesIndex]);
	}

}

void ABuildingVisual::SetBuildPosition(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		SetActorHiddenInGame(false);
		SetActorLocation(HitResult.Location);
	}
	else
	{
		SetActorHiddenInGame(true);
	}
}

void ABuildingVisual::SpawnBuilding()
{
	if (BuildingClass && !IsHidden())
	{
		GetWorld()->SpawnActor<ABuilding>(BuildingClass, GetActorTransform());
	}
}

void ABuildingVisual::CycleMesh()
{
	if (++BuildingMeshesIndex >= BuildingMeshes.Num())
	{
		BuildingMeshesIndex = 0;
	}

	if (BuildingMeshes[BuildingMeshesIndex])
	{
		BuildMesh->SetStaticMesh(BuildingMeshes[BuildingMeshesIndex]);
	}
}

