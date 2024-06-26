// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"
#include "Components/InstancedStaticMeshComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

// Sets default values
ABuilding::ABuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// ����ƽ �޽� �Ҵ��ϱ�
	FoundationInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("FoundationInstancedStaticMeshComponent"));
	RootComponent = FoundationInstancedMesh;

	WallInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WallInstancedStaticMeshComponent"));

	CeilingInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("CeilingInstancedStaticMeshComponent"));

}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();

	FInstanceSocketCheck InstanceSocket;

	InstanceSocket.InstancedComponent = FoundationInstancedMesh;
	InstanceSocketsCheck.Add(InstanceSocket);

	InstanceSocket.InstancedComponent = WallInstancedMesh;
	InstanceSocketsCheck.Add(InstanceSocket);

	InstanceSocket.InstancedComponent = CeilingInstancedMesh;
	InstanceSocketsCheck.Add(InstanceSocket);

	FBuildingSocketData BuildingSocketData;
	BuildingSocketData.Index = 0;
	BuildingSocketData.InstancedComponent = FoundationInstancedMesh;
	BuildingSocketData.SocketName = NAME_None;
	BuildingSocketData.SocketTransform = GetActorTransform();
	AddInstance(BuildingSocketData, EBuildType::Foundation);

	// B Ű�� ������ �� �̸����� �޽� ǥ���ϱ�
	//FoundationInstancedMesh->AddInstance(FTransform());

	MeshInstancedSockets = FoundationInstancedMesh->GetAllSocketNames();
	MeshInstancedSockets.Append(WallInstancedMesh->GetAllSocketNames());
	MeshInstancedSockets.Append(CeilingInstancedMesh->GetAllSocketNames());	
}



void ABuilding::DestroyInstance(FVector HitPoint)
{
	const TArray<int32> HitIndexes = FoundationInstancedMesh->GetInstancesOverlappingSphere(HitPoint, 1.0f);

	if (HitIndexes.Num() > 0)
	{
		// ��Ʈ���� �α�
		UE_LOG(LogTemp, Warning, TEXT ("Destroy Instances HitIndexes"));

		FoundationInstancedMesh->RemoveInstance(HitIndexes[0]);
	}
}

FTransform ABuilding::GetInstancedSocketTransform(UInstancedStaticMeshComponent* InstancedComponent, int32 InstanceIndex, const FName& SocketName)
{
	
	if (InstancedComponent && InstancedComponent->IsValidInstance(InstanceIndex))
	{
		// �ν��Ͻ��� ��ġ���� ��� ����
		FTransform InstanceTransform = FTransform(); 
		InstancedComponent->GetInstanceTransform(InstanceIndex, InstanceTransform, true);
		
		// �ν��Ͻ� ������Ʈ�� ���� ��ġ�� ��� ����
		FTransform SocketTransform = InstancedComponent->GetSocketTransform(SocketName, RTS_Component);
		InstanceTransform = SocketTransform * InstanceTransform;

 		DrawDebugString(GetWorld(), InstanceTransform.GetLocation(), SocketName.ToString(), nullptr, FColor::White, 0.01f);
 		DrawDebugSphere(GetWorld(), InstanceTransform.GetLocation(), 5.0f, 10, FColor::Red);
 		FTransform Temp;
 		InstancedComponent->GetInstanceTransform(InstanceIndex, Temp, true);
 		DrawDebugSphere(GetWorld(), Temp.GetLocation(), 5.0f, 15, FColor::Blue);
		
		return InstanceTransform;

// 		// �ν��Ͻ� ���� ��ġ�� ��, ������ ��ġ�Ǿ� �ִ� �ν��Ͻ��� ���� ��ġ�� ã�Ƽ� ���� ��ġ�� �ν��Ͻ��� ���� ��ġ �ޱ�
// 		if (SocketTransform.Equals(FTransform()))
// 		{
// 			Success = false;
// 			return FTransform();
// 		}
// 
// 
// 		FTransform RelativeTransform = UKismetMathLibrary::MakeRelativeTransform(SocketTransform, InstanceTransform);
// 		FVector RelativeLocation = RelativeTransform.GetLocation();
// 		// ��ġ �� �ν��Ͻ��� ���� ���忡 ó�� ��ġ �� ��, ��ġ �� ��ġ �ޱ�
// 		if (WorldSpace)
// 		{			
// 			RelativeLocation.Z = SocketTransform.GetLocation().Z;
// 			InstancedComponent->GetInstanceTransform(InstanceIndex, InstanceTransform, true);
// 			
// 			FVector WorldLocation = InstanceTransform.GetLocation() + RelativeLocation;
// 			RelativeTransform.SetLocation(WorldLocation);
// 			DrawDebugString(GetWorld(), RelativeTransform.GetLocation(), SocketName.ToString(), nullptr, FColor::White, 0.01f);
// 			DrawDebugSphere(GetWorld(), RelativeTransform.GetLocation(), 5.0f, 10, FColor::Red);
// 			return RelativeTransform;
// 		}
// 
// 		RelativeLocation.Z = InstanceTransform.GetLocation().Z + SocketTransform.GetLocation().Z;		
// 		RelativeTransform.SetLocation(RelativeLocation);
// 
// 		return RelativeTransform;
	}
	
	return FTransform();
}


int32 ABuilding::GetHitIndex(const FHitResult& HitResult)
{
	DrawDebugSphere(GetWorld(), HitResult.Location, 10.0f, 10, FColor::Red);

	return HitResult.Item;
}

bool ABuilding::IsValidSocket(UInstancedStaticMeshComponent* HitComponent, const FName& Filter, const FName& SocketName)
{
	bool bSuccess = true;
	if (!HitComponent->DoesSocketExist(SocketName))
	{
		bSuccess = false;
		return bSuccess;
	}
	FString FilterString = Filter.ToString();
	FString SocketNameString = SocketName.ToString();

	if (!SocketNameString.Contains(FilterString, ESearchCase::CaseSensitive))
	{
		bSuccess = false;
	}

	return bSuccess;
}

FBuildingSocketData ABuilding::GetHitSocketTransform(const FHitResult& HitResult, const FName& Filter, float ValidHitDistance)
{
	FBuildingSocketData SocketData = FBuildingSocketData();

	if (UInstancedStaticMeshComponent* HitComponent = Cast<UInstancedStaticMeshComponent>(HitResult.GetComponent()))
	{

		// ���� ����� ������ �Ǻ��ϱ�
		int32 HitIndex = GetHitIndex(HitResult);

		if (HitIndex != -1)
		{		
			for(const FName& SocketName : MeshInstancedSockets)
			{
				if (IsValidSocket(HitComponent, Filter, SocketName))
				{
					FTransform SocketTransform = GetInstancedSocketTransform(HitComponent, HitIndex, SocketName);
					if (FVector::Distance(SocketTransform.GetLocation(), HitResult.Location) <= ValidHitDistance)
					{
						//UE_LOG(LogTemp, Warning, TEXT("Valid Hit On Socket: %s"), *SocketName.ToString
						SocketData.Index = HitIndex;
						SocketData.InstancedComponent = HitComponent;
						SocketData.SocketName = SocketName;
						SocketData.SocketTransform = SocketTransform;
						return SocketData;
					}
				}		
			}
		}
	}
	return SocketData;
}


void ABuilding::AddInstance(const FBuildingSocketData& BuildingSocketData, EBuildType BuildType)
{
	for (FInstanceSocketCheck& InstanceSocket : InstanceSocketsCheck)
	{
		if (InstanceSocket.InstancedComponent == BuildingSocketData.InstancedComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Matching Component : %s"), *InstanceSocket.InstancedComponent->GetName());

			FBuildIndexSockets BuildIndexSockets;
			BuildIndexSockets.Index = BuildingSocketData.Index;

			FSocketInformation SocketInformation;

			for (const FName& SocketName : InstanceSocket.InstancedComponent->GetAllSocketNames())
			{
				SocketInformation.SocketName = SocketName.ToString();
				if (SocketName.IsEqual(BuildingSocketData.SocketName))
				{
					SocketInformation.bSocketInUse = true;
				}
				BuildIndexSockets.SocketsInformation.Add(SocketInformation);
			}
			InstanceSocket.InstanceSocketInformation.Add(BuildIndexSockets);
		}
	}

	switch (BuildType)
	{
	case EBuildType::Foundation: FoundationInstancedMesh->AddInstanceWorldSpace(BuildingSocketData.SocketTransform); break;
	case EBuildType::Wall: WallInstancedMesh->AddInstanceWorldSpace(BuildingSocketData.SocketTransform); break;
	case EBuildType::Ceiling: CeilingInstancedMesh->AddInstanceWorldSpace(BuildingSocketData.SocketTransform); break;
	}
}
