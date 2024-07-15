// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingVisual.h"
#include "Building.h"
#include "Components/StaticMeshComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h>
#include "Player/PixelCodeCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h>


// Sets default values
ABuildingVisual::ABuildingVisual()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	BuildMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildMeshComponent"));
	RootComponent = BuildMesh;

	BuildingTypeIndex = 0;

	bMaterialIsTrue = false;
	bReturnedMesh = true;
}

// Called when the game starts or when spawned
void ABuildingVisual::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorHiddenInGame(true);

	if (BuildingTypes[BuildingTypeIndex].BuildingMesh)
	{
		BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh);
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

void ABuildingVisual::SetMeshTo(EBuildType BuildType)
{
// 	UE_LOG(LogTemp, Warning, TEXT("SetMeshTo"));
// 	bReturnedMesh = false;
// 	for (const FBuildingVisualType& Building : BuildingTypes)
// 	{
// 		if (Building.BuildType == BuildType)
// 		{
// 			BuildMesh->SetStaticMesh(Building.BuildingMesh);
// 			return;
// 		}
// 	}
	ServerRPC_SetMeshTo(BuildType);
}

void ABuildingVisual::ServerRPC_SetMeshTo_Implementation(EBuildType BuildType)
{
	UE_LOG(LogTemp, Warning, TEXT("SetMeshTo"));
	bReturnedMesh = false;
	for (const FBuildingVisualType& Building : BuildingTypes)
	{
		if (Building.BuildType == BuildType)
		{
			BuildMesh->SetStaticMesh(Building.BuildingMesh);
			return;
		}
	}
	NetMultiRPC_SetMeshTo(BuildType);
}

void ABuildingVisual::NetMultiRPC_SetMeshTo_Implementation(EBuildType BuildType)
{
	UE_LOG(LogTemp, Warning, TEXT("SetMeshTo"));
	bReturnedMesh = false;
	for (const FBuildingVisualType& Building : BuildingTypes)
	{
		if (Building.BuildType == BuildType)
		{
			BuildMesh->SetStaticMesh(Building.BuildingMesh);
			return;
		}
	}
}

void ABuildingVisual::ReturnMeshToSelected()
{
	UE_LOG(LogTemp, Warning, TEXT("ReturnMeshToSelected"));

	bReturnedMesh = true;
	if (BuildingTypes[BuildingTypeIndex].BuildingMesh)
	{
		BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh);
	}
}

void ABuildingVisual::SetBuildPosition(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		SetActorHiddenInGame(false);
		InteractingBuilding = GetHitBuildingActor(HitResult); 

		// #19 건축 자재 스냅시키기
		if (InteractingBuilding)
		{
			if (!bReturnedMesh)
			{
				ReturnMeshToSelected();
			}
			
			SocketData = InteractingBuilding->GetHitSocketTransform(HitResult, BuildingTypes[BuildingTypeIndex].FilterCharacter, 25.0f);

			if (!SocketData.SocketTransform.Equals(FTransform()))
			{
				SetActorTransform(SocketData.SocketTransform);
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
				SetActorLocationAndRotation(HitResult.Location,	GetActorRotation());
			}
		}
		else
		{
			if (bReturnedMesh)
			{
				SetMeshTo(EBuildType::Foundation);
			}

			SetActorLocation(HitResult.ImpactPoint);
		}
	}
	else
	{
		InteractingBuilding = nullptr;
		SetActorHiddenInGame(true);
	}
}

void ABuildingVisual::SpawnBuilding()
{
	//UE_LOG(LogTemp, Warning, TEXT("---------------------BUILDINGVISUAL SpawnBuilding"));

	//FString StrBuildingClass = BuildingClass ? TEXT("true") : TEXT("false");
	//UE_LOG(LogTemp, Warning, TEXT("------------------BUILDINGVISUAL BuildingClass : %s"), *StrBuildingClass);

	//FString StrInteractingBuilding = InteractingBuilding ? TEXT("true") : TEXT("false");
	//UE_LOG(LogTemp, Warning, TEXT("------------------BUILDINGVISUAL InteractingBuilding : %s"), *StrInteractingBuilding);

	// ABuilding 이 숨김이 아닐 때 = 건축자재가 preview 상태일 때
	if (BuildingClass && !IsHidden())
		// IsHidden() --> return bHidden;
	{
		// ABuilding 인스턴스 = 건축자재가 있을 때
		if (InteractingBuilding)
		{
			// preview가 초록일 때
			if (bMaterialIsTrue)
			{
				// ABuilding 클래스의 AddInstance() 호출
 				InteractingBuilding->AddInstance(SocketData, BuildingTypes[BuildingTypeIndex].BuildType);
				UE_LOG(LogTemp, Warning, TEXT("---------------------BUILDINGVISUAL Add Instance"));
			}
		}
		else
		{
			GetWorld()->SpawnActor<ABuilding>(BuildingClass, GetActorTransform());
			UE_LOG(LogTemp, Warning, TEXT("---------------------BUILDINGVISUAL Spawn Actor"));

		}
	}
}

void ABuildingVisual::DestroyInstance(const FHitResult& HitResult)
{
	if (InteractingBuilding)
	{
		if ( UInstancedStaticMeshComponent* InstancedStaticMeshComponent = Cast<UInstancedStaticMeshComponent>(HitResult.GetComponent()))
		{
			FBuildingSocketData BuildingSocketData;
			BuildingSocketData.InstancedComponent = InstancedStaticMeshComponent;
			BuildingSocketData.Index = HitResult.Item;

			InteractingBuilding->DestroyInstance(BuildingSocketData);
		}
	}
}

void ABuildingVisual::CycleMesh()
{
	UE_LOG(LogTemp, Warning, TEXT("------------------------------------------------------------------------BUILDINGVISUAL CycleMesh"));

	if (bReturnedMesh)
	{
		// 건축자재 인덱스 스크롤로 돌리기
		FString StrBuildingTypes = BuildingTypeIndex ? TEXT("true") : TEXT("false");
		UE_LOG(LogTemp, Warning, TEXT("************************CYCLE MESH BuildingTypeIndex : %s"), *StrBuildingTypes);

		FString StrBuildMesh = BuildMesh ? TEXT("true") : TEXT("false");
		UE_LOG(LogTemp, Warning, TEXT("************************CYCLE MESH BuildMesh : %s"), *StrBuildMesh);

		if (++BuildingTypeIndex >= BuildingTypes.Num())
		{
			BuildingTypeIndex = 0;
		}
		// 건축자재[#]의 메시를 preview 메시로 설정하기
		if (BuildingTypes[BuildingTypeIndex].BuildingMesh)
		{
			BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh);
			UE_LOG(LogTemp, Warning, TEXT("------------------------------------------------------------------------BUILDINGVISUAL CYCLE MESH SetStaticMesh"));

		}
	}	
}

void ABuildingVisual::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABuildingVisual, BuildingTypeIndex);
	DOREPLIFETIME(ABuildingVisual, BuildMesh);
	DOREPLIFETIME(ABuildingVisual, BuildingClass);
	DOREPLIFETIME(ABuildingVisual, BuildingTypes);
	DOREPLIFETIME(ABuildingVisual, MaterialFalse);
	DOREPLIFETIME(ABuildingVisual, MaterialTrue);
	DOREPLIFETIME(ABuildingVisual, bMaterialIsTrue);
	DOREPLIFETIME(ABuildingVisual, InteractingBuilding);
	DOREPLIFETIME(ABuildingVisual, SocketData);
	DOREPLIFETIME(ABuildingVisual, bReturnedMesh);
}


