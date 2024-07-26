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
		BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh); // �ε��� �ʱ� ������ 0 = Foundation
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
 	UE_LOG(LogTemp, Warning, TEXT("SetMeshTo :"));

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
		auto Pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (Pc)
		{
			pc = Cast<APixelCodeCharacter>(Pc->GetPawn());

			FRotator ControlRotation = Pc->GetControlRotation();

			// Z�� ȸ���� ��Ʈ�ѷ��� ȸ������ ����
			FRotator NewRotation = GetActorRotation();
			NewRotation.Yaw = ControlRotation.Yaw;
			SetActorRotation(NewRotation);
			//UE_LOG(LogTemp, Warning, TEXT("+++++++++++++++++++++++++++++++++++++++"));
		}
		SetActorHiddenInGame(false); // BuildingVisual ������ ���̱�
		InteractingBuilding = GetHitBuildingActor(HitResult); // HitResult�� Building�� Actor�� ĳ����
 
 		// #19 ���� ���� ������Ű��
 		if (InteractingBuilding)
 		{
 			FString sMode = bReturnedMesh ? TEXT("ReturnMesh : True") : TEXT("ReturnMesh : False");
 			//UE_LOG(LogTemp, Warning, TEXT("------------------ %s"), *sMode);
 
 			if (!bReturnedMesh)
 			{
 				ReturnMeshToSelected(); // ���⼭ bReturnMesh = true
 			}
 			
 			SocketData = InteractingBuilding->GetHitSocketTransform(HitResult, BuildingTypes[BuildingTypeIndex].FilterCharacter, 70.0f); // ���� ���� ����
 
 			if (!SocketData.SocketTransform.Equals(FTransform()))
 			{
 				SetActorTransform(SocketData.SocketTransform); // �ش� ���Ͽ� �´� �������� ȸ��!!
 
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
 				SetActorLocation/*AndRotation*/(HitResult.Location/*, GetActorRotation()*/); // ����Ʈ���̽� �Ÿ� �ȿ� ���� �޽��� �����Ǹ� ���� �����͸� �޾� �����ñ��
  			}
 		}
 		else
 		{
 			if (bReturnedMesh)
 			{
//  	&%&%&%			if (BuildingTypes[BuildingTypeIndex].BuildingMesh)
//  				{
//  					BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh);
//  					//BuildMesh->SetStaticMesh(ItemReference->Buildtypes[BuildingTypeIndex].BuildingMesh);
//  	&%&%&%			}
 			}

			Loc = HitResult.ImpactPoint;
			NewLoc = Loc;
			NewLoc.Z =Loc.Z + 40.f;
//  			SetActorLocation(HitResult.ImpactPoint);
			SetActorLocation(NewLoc);
 		}
	}
	else
	{
		InteractingBuilding = nullptr;
		SetActorHiddenInGame(true); // ����Ʈ���̽� �Ÿ� �ȿ� HitResult�� ������ ������ ����
	}
}

void ABuildingVisual::SpawnBuilding()
{

	// ABuilding �� ������ �ƴ� �� = �������簡 preview ������ ��
	if (BuildingClass && !IsHidden())
	{
		// ABuilding �ν��Ͻ� = �������簡 ���� ��
		if (InteractingBuilding)
		{
			// preview�� �ʷ��� ��
			if (bMaterialIsTrue)
			{
				// ABuilding Ŭ������ AddInstance() ȣ��
 				InteractingBuilding->AddInstance(SocketData, BuildingTypes[BuildingTypeIndex].BuildType);
				UE_LOG(LogTemp, Warning, TEXT("---------------------BUILDINGVISUAL Add Instance"));
			}
		}
		else
		{
			GetWorld()->SpawnActor<ABuilding>(BuildingClass, Loc, GetActorRotation());
			UE_LOG(LogTemp, Warning, TEXT("---------------------BUILDINGVISUAL Spawn Actor"));
		}
	}
}

void ABuildingVisual::DestroyInstance(const FHitResult& HitResult)
{
	if (InteractingBuilding)
	{
		if (HitResult.bBlockingHit)
		{
			if (UInstancedStaticMeshComponent* InstancedStaticMeshComponent = Cast<UInstancedStaticMeshComponent>(HitResult.GetComponent()))
			{
				FBuildingSocketData BuildingSocketData;
				BuildingSocketData.InstancedComponent = InstancedStaticMeshComponent;
				BuildingSocketData.Index = HitResult.Item;

				InteractingBuilding->DestroyInstance(BuildingSocketData, HitResult);
			}

			if(UStaticMeshComponent* statickMesh = Cast<UStaticMeshComponent>(HitResult.GetActor()))
			{
				statickMesh->DestroyComponent();
			}
			
// 			auto Pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
// 
// 			if (Pc)
// 			{
// 				UE_LOG(LogTemp, Warning, TEXT("Controller Exist"));
// 				pc = Cast<APixelCodeCharacter>(Pc->GetPawn());
// 				pc->NetMulticastRPC_DestroyBuildingInstance(HitResult);
// 			}
		}
	}
}

void ABuildingVisual::CycleMesh()
{
	FString sMode = bReturnedMesh ? TEXT("ReturnMesh : True") : TEXT("ReturnMesh : False");
	UE_LOG(LogTemp, Warning, TEXT("------------------ %s"), *sMode);

//  	if (bReturnedMesh) //$$
//  	{
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
// 	}
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


