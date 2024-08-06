#include "Building.h"
#include "Components/InstancedStaticMeshComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h>
#include "Player/PixelCodeCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "PCodeSaveGame.h"

ABuilding::ABuilding()
{
	PrimaryActorTick.bCanEverTick = false;

	BaseInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("FoundationInstancedStaticMeshComponent"));
	RootComponent = BaseInstancedMesh;
	BaseInstancedMesh->SetIsReplicated(true);

	WallInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WallInstancedStaticMeshComponent"));
	WallInstancedMesh->SetIsReplicated(true);


	CeilingInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("CeilingInstancedStaticMeshComponent"));
	CeilingInstancedMesh->SetIsReplicated(true);

	RoofInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("RoofInstancedStaticMeshComponent"));
	RoofInstancedMesh->SetIsReplicated(true);

	GableInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("GableInstancedStaticMeshComponent"));
	GableInstancedMesh->SetIsReplicated(true);

	StairsInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("StairsInstancedStaticMeshComponent"));
	StairsInstancedMesh->SetIsReplicated(true);

	WindowInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WindowInstancedStaticMeshComponent"));
	WindowInstancedMesh->SetIsReplicated(true);

	ArchInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ArchInstancedStaticMeshComponent"));
	ArchInstancedMesh->SetIsReplicated(true);

	FloorInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("FloorInstancedStaticMeshComponent"));
	FloorInstancedMesh->SetIsReplicated(true);

	bReplicates = true;
	bAlwaysRelevant = true;
}

void ABuilding::BeginPlay()
{
	Super::BeginPlay();

	FInstanceSocketCheck InstanceSocket;

	InstanceSocket.InstancedComponent = BaseInstancedMesh;
	InstanceSocketsCheck.Add(InstanceSocket);

	InstanceSocket.InstancedComponent = WallInstancedMesh;
	InstanceSocketsCheck.Add(InstanceSocket);

	InstanceSocket.InstancedComponent = CeilingInstancedMesh;
	InstanceSocketsCheck.Add(InstanceSocket);

	InstanceSocket.InstancedComponent = RoofInstancedMesh;
	InstanceSocketsCheck.Add(InstanceSocket);

	InstanceSocket.InstancedComponent = GableInstancedMesh;
	InstanceSocketsCheck.Add(InstanceSocket);

	InstanceSocket.InstancedComponent = StairsInstancedMesh;
	InstanceSocketsCheck.Add(InstanceSocket);

	InstanceSocket.InstancedComponent = WindowInstancedMesh;
	InstanceSocketsCheck.Add(InstanceSocket);

	InstanceSocket.InstancedComponent = ArchInstancedMesh;
	InstanceSocketsCheck.Add(InstanceSocket);

	InstanceSocket.InstancedComponent = FloorInstancedMesh;
	InstanceSocketsCheck.Add(InstanceSocket);

	FBuildingSocketData BuildingSocketData;
	BuildingSocketData.Index = 0;
	BuildingSocketData.InstancedComponent = BaseInstancedMesh;
	BuildingSocketData.SocketName = NAME_None;
	BuildingSocketData.SocketTransform = GetActorTransform();
	AddInstance(BuildingSocketData, EBuildType::Base);

	MeshInstancedSockets = BaseInstancedMesh->GetAllSocketNames();
	MeshInstancedSockets.Append(WallInstancedMesh->GetAllSocketNames());
	MeshInstancedSockets.Append(CeilingInstancedMesh->GetAllSocketNames());	
	MeshInstancedSockets.Append(RoofInstancedMesh->GetAllSocketNames());
	MeshInstancedSockets.Append(GableInstancedMesh->GetAllSocketNames());
	MeshInstancedSockets.Append(StairsInstancedMesh->GetAllSocketNames());
	MeshInstancedSockets.Append(WindowInstancedMesh->GetAllSocketNames());
	MeshInstancedSockets.Append(ArchInstancedMesh->GetAllSocketNames());
	MeshInstancedSockets.Append(FloorInstancedMesh->GetAllSocketNames());
}

void ABuilding::DestroyInstance(const FBuildingSocketData& BuildingSocketData)
{	
	//UE_LOG(LogTemp, Warning, TEXT("------------------------------------------------------------------------BUILDING DestroyInstance"));
	
 	if (BuildingSocketData.InstancedComponent)
 	{
		BuildingSocketData.InstancedComponent->RemoveInstance(BuildingSocketData.Index);
		//int32 index = BuildingSocketData.Index;

		for (FInstanceSocketCheck& InstanceSocket : InstanceSocketsCheck)
		{
			if (InstanceSocket.InstancedComponent == BuildingSocketData.InstancedComponent) // 인스턴스 소켓의 메시가 = 인자로 넘겨받은 소켓의 메시랑 같을 때
			{
				for (FBuildIndexSockets& IndexSockets : InstanceSocket.InstanceSocketInformation)
				{
					if (IndexSockets.Index == BuildingSocketData.Index)
					{
						for (FSocketInformation& SocketInformation : IndexSockets.SocketsInformation)
						{
							if (SocketInformation.SocketName == BuildingSocketData.SocketName/*$.ToString()*/)
							{
								SocketInformation.bSocketInUse = false;
								break;
							}
						}
						break;
					}
				}
			}
		}
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

 		//DrawDebugString(GetWorld(), InstanceTransform.GetLocation(), SocketName.ToString(), nullptr, FColor::White, 0.01f);
 		//DrawDebugSphere(GetWorld(), InstanceTransform.GetLocation(), 8.0f, 10, FColor::Red);
 		FTransform Temp;
 		InstancedComponent->GetInstanceTransform(InstanceIndex, Temp, true);
 		//DrawDebugSphere(GetWorld(), Temp.GetLocation(), 8.0f, 15, FColor::Blue);
		
		return InstanceTransform;
	}	
	return FTransform();
}

int32 ABuilding::GetHitIndex(const FHitResult& HitResult)
{
	//DrawDebugSphere(GetWorld(), HitResult.Location, 10.0f, 10, FColor::Red);
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
		if (InstanceSocket.InstancedComponent == BuildingSocketData.InstancedComponent) // 인스턴스 소켓의 메시가 = 인자로 넘겨받은 소켓의 메시랑 같을 때
		{
			bool bFoundMatchingIndex = false;
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
							break;
						}
					}
					break;
				}
			}

			if (!bFoundMatchingIndex)
			{
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
					}
					BuildIndexSockets.SocketsInformation.Add(SocketInformation);
				}
				InstanceSocket.InstanceSocketInformation.Add(BuildIndexSockets);
			}	
		}
	}

 	FTransform transform = BuildingSocketData.SocketTransform; 
	//bool bAddInstanceSuccess=false;

	//switch (BuildType)
	//{		
	//	case EBuildType::Base:
	//	BaseInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
	//	StrBuildType=TEXT("FoundationInstancedMesh");
	//	bAddInstanceSuccess = true;
	//	break;

	//	case EBuildType::Wall:
	//	WallInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
	//	StrBuildType = TEXT("WallInstancedMesh");
	//	bAddInstanceSuccess = true;
	//	break;

	//	case EBuildType::Ceiling:
	//	CeilingInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
	//	StrBuildType = TEXT("CeilingInstancedMesh");
	//	bAddInstanceSuccess = true;
	//	break;

	//	case EBuildType::Roof:
	//	RoofInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
	//	StrBuildType = TEXT("RoofInstancedMesh");
	//	bAddInstanceSuccess = true;
	//	break;

	//	case EBuildType::Gable:
	//	GableInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
	//	StrBuildType = TEXT("GableInstancedMesh");
	//	bAddInstanceSuccess = true;
	//	break;

	//	case EBuildType::Stairs:
	//	StairsInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
	//	StrBuildType = TEXT("StairsInstancedMesh");
	//	bAddInstanceSuccess = true;
	//	break;

	//	case EBuildType::Window:
	//		WindowInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
	//		StrBuildType = TEXT("WindowInstancedMesh");
	//		bAddInstanceSuccess = true;
	//		break;

	//	case EBuildType::Arch:
	//		ArchInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
	//		StrBuildType = TEXT("ArchInstancedMesh");
	//		bAddInstanceSuccess = true;
	//		break;	
	//		
	//	case EBuildType::Floor:
	//		FloorInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
	//		StrBuildType = TEXT("FloorInstancedMesh");
	//		bAddInstanceSuccess = true;
	//		break;

	//	default:
	//	break;
	//}

 	auto Pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
 	if (Pc)
 	{
 		pc = Cast<APixelCodeCharacter>(Pc->GetPawn());
		pc->NetMulticastRPC_SpawnBuilding(BuildingSocketData, BuildType, transform);
		pc->AGetSpecificBuildingAmount(BuildType);
 	}	
}

void ABuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABuilding, MeshInstancedSockets);
	DOREPLIFETIME(ABuilding, InstanceSocketsCheck);
	DOREPLIFETIME(ABuilding, BaseInstancedMesh);
	DOREPLIFETIME(ABuilding, WallInstancedMesh);
	DOREPLIFETIME(ABuilding, CeilingInstancedMesh);
	DOREPLIFETIME(ABuilding, RoofInstancedMesh);
	DOREPLIFETIME(ABuilding, GableInstancedMesh);
	DOREPLIFETIME(ABuilding, StairsInstancedMesh);
	DOREPLIFETIME(ABuilding, WindowInstancedMesh);
	DOREPLIFETIME(ABuilding, ArchInstancedMesh);
	DOREPLIFETIME(ABuilding, FloorInstancedMesh);
}
