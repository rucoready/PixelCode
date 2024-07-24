#include "Building.h"
#include "Components/InstancedStaticMeshComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h>
#include "Player/PixelCodeCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

ABuilding::ABuilding()
{
	PrimaryActorTick.bCanEverTick = false;

	FoundationInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("FoundationInstancedStaticMeshComponent"));
	RootComponent = FoundationInstancedMesh;
	FoundationInstancedMesh->SetIsReplicated(true);

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

	bReplicates = true;
	bAlwaysRelevant = true;
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

	FBuildingSocketData BuildingSocketData;
	BuildingSocketData.Index = 0;
	BuildingSocketData.InstancedComponent = FoundationInstancedMesh;
	BuildingSocketData.SocketName = NAME_None;
	BuildingSocketData.SocketTransform = GetActorTransform();
	AddInstance(BuildingSocketData, EBuildType::Foundation);

	MeshInstancedSockets = FoundationInstancedMesh->GetAllSocketNames();
	MeshInstancedSockets.Append(WallInstancedMesh->GetAllSocketNames());
	MeshInstancedSockets.Append(CeilingInstancedMesh->GetAllSocketNames());	
	MeshInstancedSockets.Append(RoofInstancedMesh->GetAllSocketNames());
	MeshInstancedSockets.Append(GableInstancedMesh->GetAllSocketNames());
	MeshInstancedSockets.Append(StairsInstancedMesh->GetAllSocketNames());
	MeshInstancedSockets.Append(WindowInstancedMesh->GetAllSocketNames());
	MeshInstancedSockets.Append(ArchInstancedMesh->GetAllSocketNames());
}

void ABuilding::DestroyInstance(const FBuildingSocketData& BuildingSocketData, const FHitResult& HitResult)
{	
	UE_LOG(LogTemp, Warning, TEXT("------------------------------------------------------------------------BUILDING DestroyInstance"));

	if (BuildingSocketData.InstancedComponent)
	{
		UInstancedStaticMeshComponent* Instance = BuildingSocketData.InstancedComponent;
		int32 num = BuildingSocketData.Index;

		//BuildingSocketData.InstancedComponent->RemoveInstance(BuildingSocketData.Index);
		UE_LOG(LogTemp, Warning, TEXT("------------------------------------------------------------------------BUILDING RemoveInstance"));

		//if (!HasAuthority())
		{
			auto Pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			//Pc->IsLocalController() = ROLE_AutonomousProxy
			if (Pc /*&& ROLE_AutonomousProxy*/)
			{
				pc = Cast<APixelCodeCharacter>(Pc->GetPawn());
				pc->NetMulticastRPC_DestroyBuildingInstance(Instance, num);
			}
		}
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
		if (InstanceSocket.InstancedComponent == BuildingSocketData.InstancedComponent) // �ν��Ͻ� ������ �޽ð� = ���ڷ� �Ѱܹ��� ������ �޽ö� ���� ��
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
 							UE_LOG(LogTemp, Warning, TEXT("---------------------------------------------------------------@@@@@@@@@@@@@@@@@@@@@@"));

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
 						UE_LOG(LogTemp, Warning, TEXT("---------------------------------------------------------------########################"));
					}
					BuildIndexSockets.SocketsInformation.Add(SocketInformation);
				}
				InstanceSocket.InstanceSocketInformation.Add(BuildIndexSockets);
			}	
		}
	}

	FTransform transform = BuildingSocketData.SocketTransform;
	FString StrBuildType;

	switch (BuildType)
	{		
		case EBuildType::Foundation:
		FoundationInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
		StrBuildType=TEXT("FoundationInstancedMesh");
// 		UE_LOG(LogTemp, Warning, TEXT("**************************************************Foundation BuildType"));
		break;

		case EBuildType::Wall:
		WallInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
		StrBuildType = TEXT("WallInstancedMesh");
// 		UE_LOG(LogTemp, Warning, TEXT("**************************************************Wall BuildType"));
		break;

		case EBuildType::Ceiling:
		CeilingInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
		StrBuildType = TEXT("CeilingInstancedMesh");
// 		UE_LOG(LogTemp, Warning, TEXT("**************************************************Ceiling BuildType"));
		break;

		case EBuildType::Roof:
		RoofInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
		StrBuildType = TEXT("RoofInstancedMesh");
// 		UE_LOG(LogTemp, Warning, TEXT("**************************************************Wooden Pilar BuildType"));
		break;

		case EBuildType::Gable:
		GableInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
		StrBuildType = TEXT("GableInstancedMesh");
		// 		UE_LOG(LogTemp, Warning, TEXT("**************************************************Wooden Pilar BuildType"));
		break;

		case EBuildType::Stairs:
		StairsInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
		StrBuildType = TEXT("StairsInstancedMesh");
		// 		UE_LOG(LogTemp, Warning, TEXT("**************************************************Wooden Pilar BuildType"));
		break;

		case EBuildType::Window:
			WindowInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
			StrBuildType = TEXT("WindowInstancedMesh");
			// 		UE_LOG(LogTemp, Warning, TEXT("**************************************************Wooden Pilar BuildType"));
			break;

		case EBuildType::Arch:
			ArchInstancedMesh->AddInstance(BuildingSocketData.SocketTransform, true);
			StrBuildType = TEXT("ArchInstancedMesh");
			// 		UE_LOG(LogTemp, Warning, TEXT("**************************************************Wooden Pilar BuildType"));
			break;

		default:
// 		UE_LOG(LogTemp, Warning, TEXT("**************************************************Unknown BuildType"));
		break;
	}
	UE_LOG(LogTemp, Warning, TEXT("------------------BuildType %s"), *StrBuildType);

 	auto Pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
 
<<<<<<< Updated upstream
 	FString StrPc = Pc ? TEXT("OOOOO") : TEXT("XXXXX");
 	//UE_LOG(LogTemp, Warning, TEXT("Pc? : %s"), *StrPc);
 
 	if (Pc && ROLE_AutonomousProxy)
 	{
 		//UE_LOG(LogTemp, Warning, TEXT("Controller Exist"));
 		pc = Cast<APixelCodeCharacter>(Pc->GetPawn());
  		pc->NetMulticastRPC_SpawnBuilding(BuildType, transform);
  		pc->AGetSpecificBuildingAmount(BuildType);
		UE_LOG(LogTemp, Warning, TEXT("55555555555555555555565656565656"));
//  		pc->ClientRPC_SpawnBuilding(BuildType, transform);
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
	DOREPLIFETIME(ABuilding, RoofInstancedMesh);
	DOREPLIFETIME(ABuilding, GableInstancedMesh);
	DOREPLIFETIME(ABuilding, StairsInstancedMesh);
	DOREPLIFETIME(ABuilding, WindowInstancedMesh);
	DOREPLIFETIME(ABuilding, ArchInstancedMesh);
}
