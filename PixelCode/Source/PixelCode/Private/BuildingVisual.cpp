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
		BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh); // 인덱스 초기 설정은 0 = Foundation
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

			// Z축 회전을 컨트롤러의 회전으로 설정
			FRotator NewRotation = GetActorRotation();
			NewRotation.Yaw = ControlRotation.Yaw;
			SetActorRotation(NewRotation);
			//UE_LOG(LogTemp, Warning, TEXT("+++++++++++++++++++++++++++++++++++++++"));
		}
		SetActorHiddenInGame(false); // BuildingVisual 프리뷰 보이기
		InteractingBuilding = GetHitBuildingActor(HitResult); // HitResult를 Building의 Actor로 캐스팅
 
 		// #19 건축 자재 스냅시키기
 		if (InteractingBuilding)
 		{
 			FString sMode = bReturnedMesh ? TEXT("ReturnMesh : True") : TEXT("ReturnMesh : False");
 			//UE_LOG(LogTemp, Warning, TEXT("------------------ %s"), *sMode);
 
 			if (!bReturnedMesh)
 			{
 				ReturnMeshToSelected(); // 여기서 bReturnMesh = true
 			}
 			
 			SocketData = InteractingBuilding->GetHitSocketTransform(HitResult, BuildingTypes[BuildingTypeIndex].FilterCharacter, 70.0f); // 소켓 감지 범위
 
 			if (!SocketData.SocketTransform.Equals(FTransform()))
 			{
 				SetActorTransform(SocketData.SocketTransform); // 해당 소켓에 맞는 방향으로 회전!!
 
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
 				SetActorLocation/*AndRotation*/(HitResult.Location/*, GetActorRotation()*/); // 라인트레이스 거리 안에 빌딩 메쉬가 감지되면 소켓 데이터를 받아 스냅시긴다
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
		SetActorHiddenInGame(true); // 라인트레이스 거리 안에 HitResult가 없으면 프리뷰 끄기
	}
}

void ABuildingVisual::SpawnBuilding()
{

	// ABuilding 이 숨김이 아닐 때 = 건축자재가 preview 상태일 때
	if (BuildingClass && !IsHidden())
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
		// 건축자재 인덱스 스크롤로 돌리기
		if (++BuildingTypeIndex >= BuildingTypes.Num())
		{
			BuildingTypeIndex = 0;
		}
		// 건축자재[#]의 메시를 preview 메시로 설정하기
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


