// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingVisual.h"
#include "Building.h"
#include "Components/StaticMeshComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h>


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
	UE_LOG(LogTemp, Warning, TEXT("1"));
}

ABuilding* ABuildingVisual::GetHitBuildingActor(const FHitResult& HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("2"));
	return Cast<ABuilding>(HitResult.GetActor());
}

void ABuildingVisual::SetMeshTo(EBuildType BuildType)
{
	UE_LOG(LogTemp, Warning, TEXT("3"));
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
	UE_LOG(LogTemp, Warning, TEXT("4"));
	UE_LOG(LogTemp, Warning, TEXT("ReturnMeshToSelected"));

	bReturnedMesh = true;
	if (BuildingTypes[BuildingTypeIndex].BuildingMesh)
	{
		BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh);
	}
}


void ABuildingVisual::SetBuildPosition(const FHitResult& HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("5"));
	if (HitResult.bBlockingHit)
	{
		SetActorHiddenInGame(false);
		InteractingBuilding = GetHitBuildingActor(HitResult);

		// #19 ���� ���� ������Ű��
		if (InteractingBuilding)
		{
			if (!bReturnedMesh)
			{
				ReturnMeshToSelected();
			}

			SocketData = InteractingBuilding->GetHitSocketTransform(HitResult, BuildingTypes[BuildingTypeIndex].FilterCharacter, 25.0f);
			InteractingBuilding->ServerRPC_GetHitSocketTransform(SocketData, HitResult, BuildingTypes[BuildingTypeIndex].FilterCharacter, 25.0f);
			if (!SocketData.SocketTransform.Equals(FTransform()))
			{
				UE_LOG(LogTemp, Warning, TEXT("wpqkf"));
				SetActorTransform(SocketData.SocketTransform);
				UE_LOG(LogTemp, Warning, TEXT("tlqkf"));
				if (MaterialTrue && !bMaterialIsTrue)
				{
					bMaterialIsTrue = true;
					BuildMesh->SetMaterial(0, MaterialTrue);
					NetMulticastRPC_SetBuildPosition(HitResult);
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
		}
		else
		{
			if (bReturnedMesh)
			{
				SetMeshTo(EBuildType::Foundation);
			}

			SetActorLocation(HitResult.Location);
		}
	}
	else
	{
		InteractingBuilding = nullptr;
		SetActorHiddenInGame(true);
	}

}

void ABuildingVisual::ServerRPC_SetBuildPosition_Implementation(const FHitResult& HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("6"));
	NetMulticastRPC_SetBuildPosition(HitResult);
	SetBuildPosition(HitResult);
}

void ABuildingVisual::NetMulticastRPC_SetBuildPosition_Implementation(const FHitResult& HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("7"));
	SetBuildPosition(HitResult);
	

	//UE_LOG(LogTemp, Warning, TEXT("Server SetBuildPosition"));	
}

void ABuildingVisual::SpawnBuilding()
{
	UE_LOG(LogTemp, Warning, TEXT("8"));
	// ABuilding �� ������ �ƴ� �� = �������簡 preview ������ ��
	//if (BuildingClass && !IsHidden())
		// IsHidden() --> return bHidden;
		UE_LOG(LogTemp, Warning, TEXT("---------------------------------------BUILDINGVISUAL 1ST IF"));
	//{
		// ABuilding �ν��Ͻ� = �������簡 ���� ��
	if (InteractingBuilding)
		{
			UE_LOG(LogTemp, Warning, TEXT("---------------------------------------BUILDINGVISUAL 2ND IF"));

			// preview�� �ʷ��� ��
	if (bMaterialIsTrue)
			{
				UE_LOG(LogTemp, Warning, TEXT("---------------------------------------BUILDINGVISUAL 3RD IF"));

				// ABuildind Ŭ������ AddInstance() ȣ��
				InteractingBuilding->ServerRPC_AddInstance(SocketData, BuildingTypes[BuildingTypeIndex].BuildType);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("---------------------------------------BUILDINGVISUAL ELSE"));
			UE_LOG(LogTemp, Warning, TEXT("tlqkf1"));
			GetWorld()->SpawnActor<ABuilding>(BuildingClass, GetActorTransform());
		}
	//}
}

void ABuildingVisual::DestroyInstance(const FHitResult& HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("9"));
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
	UE_LOG(LogTemp, Warning, TEXT("10"));
	if (bReturnedMesh)
	{
		// �������� �ε��� ��ũ�ѷ� ������
		if (++BuildingTypeIndex >= BuildingTypes.Num())
		{
			BuildingTypeIndex = 0;
		}
		// ��������[#]�� �޽ø� preview �޽÷� �����ϱ�
		if (BuildingTypes[BuildingTypeIndex].BuildingMesh)
		{
			BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh);
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

