#include "Building.h"
#include "Components/InstancedStaticMeshComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h>

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
		// ���� ����� ������ �Ǻ��ϱ�
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
		if (InstanceSocket.InstancedComponent == BuildingSocketData.InstancedComponent)
		{
			bool bFoundMatchingIndex = false;
 			FString StrFoundMatchingIndex = bFoundMatchingIndex ? TEXT("true") : TEXT("false");
 			UE_LOG(LogTemp, Warning, TEXT("---------------------------------------------------------------The value of bFoundMatchingIndex is: %s"), *StrFoundMatchingIndex);
			for (FBuildIndexSockets& IndexSockets : InstanceSocket.InstanceSocketInformation)
			{
				if (IndexSockets.Index == BuildingSocketData.Index)
				{
					bFoundMatchingIndex = true;
					for (FSocketInformation& SocketInformation : IndexSockets.SocketsInformation)
					{
						if (SocketInformation.SocketName == BuildingSocketData.SocketName/*$.ToString()*/)
						{
							SocketInformation.bSocketInUse = true;
							UE_LOG(LogTemp, Warning, TEXT("---------------------------------------------------------------@@@@@@@@@@@@@@@@@@@@@@"));

							break;
						}
					}
					break;
				}
			}

			if (!bFoundMatchingIndex)
			{
 				UE_LOG(LogTemp, Warning, TEXT("---------------------------------------------------------------222222222222The value of bFoundMatchingIndex is: %s"), *StrFoundMatchingIndex);

				FBuildIndexSockets BuildIndexSockets;
				BuildIndexSockets.Index = BuildingSocketData.Index;

				FSocketInformation SocketInformation;

				for (const FName& SocketName : InstanceSocket.InstancedComponent->GetAllSocketNames())
				{
					SocketInformation.SocketName = SocketName.ToString();
					SocketInformation.bSocketInUse = false;
					if (SocketName.IsEqual(BuildingSocketData.SocketName))
					{
						SocketInformation.bSocketInUse = true;
						UE_LOG(LogTemp, Warning, TEXT("---------------------------------------------------------------########################"));
					}
					BuildIndexSockets.SocketsInformation.Add(SocketInformation);
				}
				InstanceSocket.InstanceSocketInformation.Add(BuildIndexSockets);
			}	
		}
	}

// 	FString StrBuildType = BuildType ? TEXT("true") : TEXT("false");
// 	UE_LOG(LogTemp, Warning, TEXT("***************************************************BuildType : %s"), *StrBuildType);
// 	switch (BuildType)
//  	{
// // 		case EBuildType::Foundation: FoundationInstancedMesh->AddInstanceWorldSpace(BuildingSocketData.SocketTransform); break;
// 		case EBuildType::Foundation: FoundationInstancedMesh->AddInstanceWorldSpace(BuildingSocketData.SocketTransform); break;
// 		UE_LOG(LogTemp, Warning, TEXT("**************************************************AddInstanceWorldSpace"));
// 
// 		case EBuildType::Wall: WallInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true); break;
// 		case EBuildType::Ceiling: CeilingInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true); break;
// 		case EBuildType::WoodenPilar: WoodenPilarInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true); break;
// 	}

	switch (BuildType)
	{
		case EBuildType::Foundation:
		FoundationInstancedMesh->AddInstanceWorldSpace(BuildingSocketData.SocketTransform);
		UE_LOG(LogTemp, Warning, TEXT("**************************************************Foundation Instance added"));
		break;

		case EBuildType::Wall:
		WallInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
		UE_LOG(LogTemp, Warning, TEXT("**************************************************Wall instance added"));
		break;

		case EBuildType::Ceiling:
		CeilingInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
		UE_LOG(LogTemp, Warning, TEXT("**************************************************Ceiling instance added"));
		break;

		case EBuildType::WoodenPilar:
		WoodenPilarInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
		UE_LOG(LogTemp, Warning, TEXT("**************************************************Wooden Pilar instance added"));
		break;

		default:
		UE_LOG(LogTemp, Warning, TEXT("**************************************************Unknown BuildType"));
		break;
	}
}

void ABuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABuilding, MeshInstancedSockets);
	DOREPLIFETIME(ABuilding, InstanceSocketsCheck);
	DOREPLIFETIME(ABuilding, FoundationInstancedMesh);
	DOREPLIFETIME(ABuilding, WallInstancedMesh);
	DOREPLIFETIME(ABuilding, CeilingInstancedMesh);
}
