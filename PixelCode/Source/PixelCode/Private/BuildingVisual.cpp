// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingVisual.h"
#include "Building.h"
#include "Components/StaticMeshComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h>
#include "Player/PixelCodeCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h>
#include "PCodeSaveGame.h"


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
 	//UE_LOG(LogTemp, Warning, TEXT("SetMeshTo :"));

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
	//UE_LOG(LogTemp, Warning, TEXT("ReturnMeshToSelected"));

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
			SetActorRotation(NewRotation);		//플레이어 시점을 따라 메시가 이동하도록
		}
		SetActorHiddenInGame(false); // BuildingVisual 프리뷰 보이기
		InteractingBuilding = GetHitBuildingActor(HitResult); // HitResult를 Building의 Actor로 캐스팅
 
 		// #19 건축 자재 스냅시키기
 		if (InteractingBuilding)
 		{
 			if (!bReturnedMesh)
 			{
 				ReturnMeshToSelected(); // 여기서 bReturnMesh = true
 			}
 			
 			SocketData = InteractingBuilding->GetHitSocketTransform(HitResult, BuildingTypes[BuildingTypeIndex].FilterCharacter, 100.0f); // 소켓 감지 범위
 
 			if (!SocketData.SocketTransform.Equals(FTransform()))
 			{
 				SetActorTransform(SocketData.SocketTransform); // 해당 소켓에 맞는 방향으로 회전!!
 
 				if (MaterialTrue && !bMaterialIsTrue)
 				{
 					bMaterialIsTrue = true;
 					BuildMesh->SetMaterial(0, MaterialTrue);		// 프리뷰 초록색으로
 				}
 				return;
 			}
 			else
 			{
 				if (MaterialFalse && bMaterialIsTrue)
 				{
 					bMaterialIsTrue = false;
 					BuildMesh->SetMaterial(0, MaterialFalse);		// 프리뷰 빨간색으로
 				}
 				SetActorLocation/*AndRotation*/(HitResult.Location/*, GetActorRotation()*/);		// 스냅배치 회전값을 초기화
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
			SetActorLocation(NewLoc);		// 땅에서 40cm 위로 프리뷰 띄우기
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
	bItemQuantityValid = false;
 	auto Pc = Cast<APlayerOrganism>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
 	if (Pc)
 	{
 		TArray<UItemBase*> InventoryContentSArray = Pc->PlayerInventory->GetInventoryContents();
 		//uint8 BulidAmount = Recipe.Amount;
 		uint8 Index = 0;
 		TArray<uint8> RemoveedIndex;
 
 		if (InventoryContentSArray.IsValidIndex(Index))
 		{
 			for (UItemBase* Item : InventoryContentSArray)
 			{
 				if (Item && Item->Buildtypes == BuildingTypes[BuildingTypeIndex].BuildType)
 				{
 					if (Item->Quantity > 0)
 					{
						bItemQuantityValid  = true;		// 아이템 수량 만큼만 빌딩 가능모드
					}
				}
			}
		}
	}
	// ABuilding 이 숨김이 아닐 때 = 건축자재가 preview 상태일 때
	if (BuildingClass && !IsHidden() && bItemQuantityValid)	// 아이템 수량 만큼만 빌딩 가능모드 -> 치트모드로 바꾸려면 !bItemQuantityValid 로 쓰기
	{
		// ABuilding 인스턴스 = 건축자재가 있을 때
		if (InteractingBuilding)
		{
			// preview가 초록일 때
			if (bMaterialIsTrue)
			{
				// ABuilding 클래스의 AddInstance() 호출
				InteractingBuilding->AddInstance(SocketData, BuildingTypes[BuildingTypeIndex].BuildType);
				//UE_LOG(LogTemp, Warning, TEXT("---------------------BUILDINGVISUAL Add Instance"));
			}
		}
		else
		{
			//GetWorld()->SpawnActor<ABuilding>(BuildingClass, Loc, GetActorRotation());
			ABuilding* AbuildingClass = GetWorld()->SpawnActor<ABuilding>(BuildingClass, Loc, GetActorRotation());
			//UE_LOG(LogTemp, Warning, TEXT("---------------------BUILDINGVISUAL Spawn Actor"));
 
								

			UPCodeSaveGame* castSave = Cast<UPCodeSaveGame>(UGameplayStatics::CreateSaveGameObject(UPCodeSaveGame::StaticClass()));
			UPCodeSaveGame* castLoad = Cast<UPCodeSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("BuildingDataStorage"), 0));
								
			if (castLoad)
			{
				FBuildingActorData BuildingActorData;
				BuildingActorData.ABuilding = AbuildingClass->GetClass();
				BuildingActorData.BuildingLocation = AbuildingClass->GetActorLocation();
				BuildingActorData.BuildingRotation = AbuildingClass->GetActorRotation();

				castLoad->SavedActors.Add(BuildingActorData);
				UGameplayStatics::SaveGameToSlot(castLoad, TEXT("BuildingDataStorage"), 0);

				int32 arrnum = castLoad->SavedActors.Num();
				//UE_LOG(LogTemp, Warning, TEXT("-------------SAVEGAME__ACTOR Load :: %d "), arrnum);
			}

 			else if (castSave)
 			{
 				FBuildingActorData BuildingActorData;
 				BuildingActorData.ABuilding = AbuildingClass->GetClass();
 				BuildingActorData.BuildingLocation = AbuildingClass->GetActorLocation();
 				BuildingActorData.BuildingRotation = AbuildingClass->GetActorRotation();
 
 				castSave->SavedActors.Add(BuildingActorData);
 				UGameplayStatics::SaveGameToSlot(castSave, TEXT("BuildingDataStorage"), 0);

				int32 arrnum = castSave->SavedActors.Num();
				//UE_LOG(LogTemp, Warning, TEXT("-------------SAVEGAME__ACTOR Cast :: %d "), arrnum);
 			}
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

				InteractingBuilding->DestroyInstance(BuildingSocketData);

			}
		}
	}
}

void ABuildingVisual::CycleMesh()
{
	//if (bReturnedMesh)
  	
		if (++BuildingTypeIndex >= BuildingTypes.Num())
 		{
 			BuildingTypeIndex = 0;
 		}
		// 건축자재[#]의 메시를 preview 메시로 설정하기
		if (BuildingTypes[BuildingTypeIndex].BuildingMesh)
		{
			BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh);
		}


// 	auto Pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
// 	if (Pc  /* ROLE_AutonomousProxy*/)
// 	{
// 		pc = Cast<APixelCodeCharacter>(Pc->GetPawn());
// 		float wheelAxisVal = pc->wheelAxis;
// 
// 		// 건축자재 인덱스 스크롤로 돌리기
// 		if(wheelAxisVal > 0)
// 		{
// 			if (BuildingTypeIndex < BuildingTypes.Num() - 1)
// 			{
// 				++BuildingTypeIndex;
// 			}
// 			else
// 			{
// 				BuildingTypeIndex = 0; // 배열의 처음으로 순환
// 			}
// 		}
// 		else if (wheelAxisVal < 0) // 스크롤 휠을 올림
// 		{
// 			if (BuildingTypeIndex > 0)
// 			{
// 				--BuildingTypeIndex;
// 			}
// 			else
// 			{
// 				BuildingTypeIndex = BuildingTypes.Num() - 1; // 배열의 끝으로 순환
// 			}
// 		}
// 
// 		// 건축자재[#]의 메시를 preview 메시로 설정하기
// 		if (BuildingTypes[BuildingTypeIndex].BuildingMesh)
// 		{
// 			BuildMesh->SetStaticMesh(BuildingTypes[BuildingTypeIndex].BuildingMesh);
// 		}
//  	}
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


