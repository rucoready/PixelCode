// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
ABuilding::ABuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FoundationInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("FoundationInstancedStaticMeshComponent"));
	RootComponent = FoundationInstancedMesh;
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	
	FTransform MeshTransform = FTransform();

	for (uint8 i = 0; i < 3; ++i)
	{
		FoundationInstancedMesh->AddInstance(MeshTransform);

		FVector MeshLocation = MeshTransform.GetLocation();
		MeshLocation.Z += 250;
		MeshTransform.SetLocation(MeshLocation);
	}
}

void ABuilding::DestroyInstance(FVector HitPoint)
{
	const TArray<int32> HitIndexes = FoundationInstancedMesh->GetInstancesOverlappingSphere(HitPoint, 1.0f);

	if (HitIndexes.Num() > 0)
	{
		FoundationInstancedMesh->RemoveInstance(HitIndexes[0]);
	}
}