#include "Building.h"
#include "Components/InstancedStaticMeshComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

ABuilding::ABuilding()
{
	PrimaryActorTick.bCanEverTick = false;

	FoundationInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("FoundationInstancedStaticMeshComponent"));
	RootComponent = FoundationInstancedMesh;

	WallInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WallInstancedStaticMeshComponent"));

	CeilingInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("CeilingInstancedStaticMeshComponent"));

	WoodenPilarInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WoodenPilarInstancedStaticMeshComponent"));
}

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

	InstanceSocket.InstancedComponent = WoodenPilarInstancedMesh;
	InstanceSocketsCheck.Add(InstanceSocket);

	FBuildingSocketData BuildingSocketData;
	BuildingSocketData.Index = 0;
	BuildingSocketData.InstancedComponent = FoundationInstancedMesh;
	BuildingSocketData.SocketName = NAME_None;
	BuildingSocketData.SocketTransform = GetActorTransform();
	AddInstance(BuildingSocketData, EBuildType::Foundation);

	MeshInstancedSockets = FoundationInstancedMesh->GetAllSocketNames();
	MeshInstancedSockets.Append(WallInstancedMesh->GetAllSocketNames());
	MeshInstancedSockets.Append(CeilingInstancedMesh->GetAllSocketNames());	
	MeshInstancedSockets.Append(WoodenPilarInstancedMesh->GetAllSocketNames());	
}

void ABuilding::DestroyInstance(const FBuildingSocketData& BuildingSocketData)
{	
	if (BuildingSocketData.InstancedComponent)
	{
		BuildingSocketData.InstancedComponent->RemoveInstance(BuildingSocketData.Index);
	}
}

FTransform ABuilding::GetInstancedSocketTransform(UInstancedStaticMeshComponent* InstancedComponent, int32 InstanceIndex, const FName& SocketName)
{	
	if (InstancedComponent && InstancedComponent->IsValidInstance(InstanceIndex))
	{
		// 인스턴스의 위치값을 담는 변수
		FTransform InstanceTransform = FTransform(); 
		InstancedComponent->GetInstanceTransform(InstanceIndex, InstanceTransform, true);
		
		// 인스턴스 컴포넌트의 소켓 위치를 담는 변수
		FTransform SocketTransform = InstancedComponent->GetSocketTransform(SocketName, RTS_Component);
		InstanceTransform = SocketTransform * InstanceTransform;

 		DrawDebugString(GetWorld(), InstanceTransform.GetLocation(), SocketName.ToString(), nullptr, FColor::White, 0.01f);
 		DrawDebugSphere(GetWorld(), InstanceTransform.GetLocation(), 5.0f, 10, FColor::Red);
 		FTransform Temp;
 		InstancedComponent->GetInstanceTransform(InstanceIndex, Temp, true);
 		DrawDebugSphere(GetWorld(), Temp.GetLocation(), 5.0f, 15, FColor::Blue);
		
		return InstanceTransform;
	}	
	return FTransform();
}

int32 ABuilding::GetHitIndex(const FHitResult& HitResult)
{
	DrawDebugSphere(GetWorld(), HitResult.Location, 10.0f, 10, FColor::Red);
	return HitResult.Item;
}

bool ABuilding::IsValidSocket(UInstancedStaticMeshComponent* HitComponent, int32 Index, const FName& Filter, const FName& SocketName)
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

	for (const FInstanceSocketCheck& InstanceSocketCheck : InstanceSocketsCheck)
	{
		if (InstanceSocketCheck.InstancedComponent == HitComponent)
		{
			for (const FBuildIndexSockets& BuildIndexSockets : InstanceSocketCheck.InstanceSocketInformation)
			{
				if (BuildIndexSockets.Index == Index)
				{
					for (const FSocketInformation& SocketInformation : BuildIndexSockets.SocketsInformation)
					{
						if (SocketInformation.SocketName == SocketName/*$.ToString()*/ && SocketInformation.bSocketInUse)
						{
							bSuccess = false;
							break;
						}
					}
				}
			}
		}
	}
	//https://youtu.be/kstSj5nU3KI?list=PLnHeglBaPYu-F1ZGmVZIfnbR11WN2_ReW&t=1406
	return bSuccess;
}

FBuildingSocketData ABuilding::GetHitSocketTransform(const FHitResult& HitResult, const FName& Filter, float ValidHitDistance)
{
	FBuildingSocketData SocketData = FBuildingSocketData();

	if (UInstancedStaticMeshComponent* HitComponent = Cast<UInstancedStaticMeshComponent>(HitResult.GetComponent()))
	{
		// 가장 가까운 소켓을 판별하기
		int32 HitIndex = GetHitIndex(HitResult);

		if (HitIndex != -1)
		{		
			for(const FName& SocketName : MeshInstancedSockets)
			{
				if (IsValidSocket(HitComponent, HitIndex, Filter, SocketName))
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
		UE_LOG(LogTemp, Warning, TEXT("---------------------------------------BUILDING 1ST FOR"));

		if (InstanceSocket.InstancedComponent == BuildingSocketData.InstancedComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("---------------------------------------BUILDING 1ST IF"));

			bool bFoundMatchingIndex = false;

			for (FBuildIndexSockets& IndexSockets : InstanceSocket.InstanceSocketInformation)
			{
				UE_LOG(LogTemp, Warning, TEXT("---------------------------------------BUILDING 2ND FOR"));

				if (IndexSockets.Index == BuildingSocketData.Index)
				{
					UE_LOG(LogTemp, Warning, TEXT("---------------------------------------BUILDING 2ND IF"));

					bFoundMatchingIndex = true;
					for (FSocketInformation& SocketInformation : IndexSockets.SocketsInformation)
					{
						UE_LOG(LogTemp, Warning, TEXT("---------------------------------------BUILDING 3RD FOR"));

						if (SocketInformation.SocketName == BuildingSocketData.SocketName/*$.ToString()*/)
						{
							UE_LOG(LogTemp, Warning, TEXT("---------------------------------------BUILDING 3RD IF"));

							// UE_LOG(LogTemp, Warning, TEXT("Setting Socket %s to TRUE"), *SocketInformation.SocketName);

							SocketInformation.bSocketInUse = true;
							break;
						}
					}
					break;
				}
			}

			if (!bFoundMatchingIndex)
			{
				UE_LOG(LogTemp, Warning, TEXT("---------------------------------------BUILDING 4TH IF"));

				//UE_LOG(LogTemp, Warning, TEXT("DID NOT FIND MATCHING INDEX"));
				//UE_LOG(LogTemp, Warning, TEXT("Found Matching Component : %s"), *InstanceSocket.InstancedComponent->GetName());

				FBuildIndexSockets BuildIndexSockets;
				BuildIndexSockets.Index = BuildingSocketData.Index;

				FSocketInformation SocketInformation;

				for (const FName& SocketName : InstanceSocket.InstancedComponent->GetAllSocketNames())
				{
					UE_LOG(LogTemp, Warning, TEXT("---------------------------------------BUILDING 4TH FOR"));

					SocketInformation.SocketName = SocketName.ToString();
					//--------------------------------------------------------------------------------------------7.2
					SocketInformation.bSocketInUse = false;
					//--------------------------------------------------------------------------------------------7.2 끝
					if (SocketName.IsEqual(BuildingSocketData.SocketName))
					{
						UE_LOG(LogTemp, Warning, TEXT("---------------------------------------BUILDING 5TH IF"));

						SocketInformation.bSocketInUse = true;
					}
					BuildIndexSockets.SocketsInformation.Add(SocketInformation);
				}
				InstanceSocket.InstanceSocketInformation.Add(BuildIndexSockets);
			}	
		}
	}

	switch (BuildType)
	{
		case EBuildType::Foundation: FoundationInstancedMesh->AddInstanceWorldSpace(BuildingSocketData.SocketTransform); break;
		case EBuildType::Wall: WallInstancedMesh->AddInstanceWorldSpace(BuildingSocketData.SocketTransform); break;
		case EBuildType::Ceiling: CeilingInstancedMesh->AddInstanceWorldSpace(BuildingSocketData.SocketTransform); break;
		case EBuildType::WoodenPilar: WoodenPilarInstancedMesh->AddInstanceWorldSpace(BuildingSocketData.SocketTransform); break;
	}
}
