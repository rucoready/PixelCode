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

	bMaterialIsTrue = false;
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

	if (MaterialTrue)
	{
		bMaterialIsTrue = true;
		BuildMesh->SetMaterial(0, MaterialTrue);
	}

}

ABuilding* ABuildingVisual::GetHitBuildingActor(const FHitResult& HitResult)
{
	return Cast<ABuilding>(HitResult.GetActor());
}

void ABuildingVisual::SetBuildPosition(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		SetActorHiddenInGame(false);

		UE_LOG(LogTemp, Warning, TEXT("SetBuildPosition"));
		// #19 건축 자재 스냅시키기
		if (ABuilding* HitBuilding = GetHitBuildingActor(HitResult))
		{
			FTransform SocketTransform = HitBuilding->GetHitSocketTransform(HitResult);
			if (!SocketTransform.Equals(FTransform()))
			{
				SetActorTransform(SocketTransform);
				if (MaterialTrue && !bMaterialIsTrue)
				{
					bMaterialIsTrue = true;
					BuildMesh->SetMaterial(0, MaterialTrue);
				}
				return;
			}
			else
			{
				if (MaterialFalse && bMaterialIsTrue)
				{
					bMaterialIsTrue = false;
					BuildMesh->SetMaterial(0, MaterialFalse);

				}
				SetActorLocation(HitResult.Location);
			}
			// 로그--------------------------------------------------------------------건축 자재 오버랩 되면 -1, 오버랩 안되면 0
			UE_LOG(LogTemp, Warning, TEXT("Hit BuildingActor"));
		}
		else
		{
			SetActorLocation(HitResult.Location);
		}
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

