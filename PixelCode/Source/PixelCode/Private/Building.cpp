// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"
#include "Components/InstancedStaticMeshComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

// Sets default values
ABuilding::ABuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 스태틱 메시 할당하기
	FoundationInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("FoundationInstancedStaticMeshComponent"));
	RootComponent = FoundationInstancedMesh;

	WallInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WallInstancedStaticMeshComponent"));

}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();

	// B 키를 눌렀을 때 미리보기 메시 표시하기
	FoundationInstancedMesh->AddInstance(FTransform());
}

void ABuilding::DestroyInstance(FVector HitPoint)
{
	const TArray<int32> HitIndexes = FoundationInstancedMesh->GetInstancesOverlappingSphere(HitPoint, 1.0f);

	if (HitIndexes.Num() > 0)
	{
		// 디스트로이 로그
		UE_LOG(LogTemp, Warning, TEXT ("Destroy Instances HitIndexes"));

		FoundationInstancedMesh->RemoveInstance(HitIndexes[0]);
	}
}

FTransform ABuilding::GetInstancedSocketTransform(UInstancedStaticMeshComponent* InstancedComponent, int32 InstanceIndex, const FName& SocketName, bool& Success, bool WorldSpace)
{
	Success = true;
	if (InstancedComponent && InstancedComponent->IsValidInstance(InstanceIndex))
	{
		// 인스턴스의 위치값을 담는 변수
		FTransform InstanceTransform = FTransform(); 
		InstancedComponent->GetInstanceTransform(InstanceIndex, InstanceTransform, false);
		
		// 인스턴스 컴포넌트의 소켓 위치를 담는 변수
		FTransform SocketTransform = InstancedComponent->GetSocketTransform(SocketName, RTS_Component);

		// 인스턴스 위에 배치할 때, 기존에 배치되어 있는 인스턴스의 소켓 위치를 찾아서 새로 배치할 인스턴스의 소켓 위치 받기
		if (SocketTransform.Equals(FTransform()))
		{
			Success = false;
			return FTransform();
		}


		FTransform RelativeTransform = UKismetMathLibrary::MakeRelativeTransform(SocketTransform, InstanceTransform);
		FVector RelativeLocation = RelativeTransform.GetLocation();
		// 배치 된 인스턴스가 없이 월드에 처음 배치 할 때, 배치 될 위치 받기
		if (WorldSpace)
		{			
			RelativeLocation.Z = SocketTransform.GetLocation().Z;
			InstancedComponent->GetInstanceTransform(InstanceIndex, InstanceTransform, true);
			
			FVector WorldLocation = InstanceTransform.GetLocation() + RelativeLocation;
			RelativeTransform.SetLocation(WorldLocation);
			return RelativeTransform;
		}
		RelativeLocation.Z = InstanceTransform.GetLocation().Z + SocketTransform.GetLocation().Z;
		
		RelativeTransform.SetLocation(RelativeLocation);

		return RelativeTransform;
	}
	Success = false;
	return FTransform();
}


int32 ABuilding::GetHitIndex(const FHitResult& HitResult)
{
	DrawDebugSphere(GetWorld(), HitResult.Location, 10.0f, 30, FColor::Red);

	return HitResult.Item;
}

FTransform ABuilding::GetHitSocketTransform(const FHitResult& HitResult, float ValidHitDistance)
{
	// 가장 가까운 소켓을 판별하기
	int32 HitIndex = GetHitIndex(HitResult);
	if (HitIndex != -1)
	{
		TArray<FName> SocketNames = FoundationInstancedMesh->GetAllSocketNames();
		bool bIsSuccessful = false;
		for(const FName& SocketName : SocketNames)
			{
				//FTransform SocketTransform = FoundationInstancedMesh->GetSocketTransform(SocketName);
				FTransform SocketTransform = GetInstancedSocketTransform(FoundationInstancedMesh, HitIndex, SocketName, bIsSuccessful, true);
				if (FVector::Distance(SocketTransform.GetLocation(), HitResult.Location) <= ValidHitDistance)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Valid Hit On Socket: %s"), *SocketName.ToString());
					return SocketTransform;
				}
			}
	}
	return FTransform();
}


void ABuilding::AddInstance(const FTransform& ActorTransform, EBuildType BuildType)
{
	switch (BuildType)
	{
		case EBuildType::Foundation : FoundationInstancedMesh->AddInstanceWorldSpace(ActorTransform); break;
		case EBuildType::Wall : WallInstancedMesh->AddInstanceWorldSpace(ActorTransform); break;
	}

}
