#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataTypes.h"
#include "Building.generated.h"

UCLASS()
class PIXELCODE_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	ABuilding();

protected:
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = KSH)
	class UInstancedStaticMeshComponent* FoundationInstancedMesh;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = KSH)
	class UInstancedStaticMeshComponent* WallInstancedMesh;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = KSH)
	class UInstancedStaticMeshComponent* CeilingInstancedMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = KSH)
	class UInstancedStaticMeshComponent* WoodenPilarInstancedMesh;

	UPROPERTY(Replicated)
	TArray<FName> MeshInstancedSockets;

	UPROPERTY(Replicated)
	TArray<FInstanceSocketCheck> InstanceSocketsCheck;

	virtual void BeginPlay() override;

	bool IsValidSocket(UInstancedStaticMeshComponent* HitComponent, int32 Index, const FName& Filter, const FName& SocketName);

public:
	UFUNCTION(BlueprintCallable, Category = KSH)
	void DestroyInstance(const FBuildingSocketData& BuildingSocketData);

	UFUNCTION(BlueprintCallable, Category = KSH)
	 FTransform GetInstancedSocketTransform(UInstancedStaticMeshComponent* InstancedComponent, int32 InstanceIndex, const FName& SocketName);

	 int32 GetHitIndex(const FHitResult& HitResult);

	 FBuildingSocketData GetHitSocketTransform(const FHitResult& HitResult, const FName& Filter, float ValidHitDistance = 100.0f);

	 void AddInstance(const FBuildingSocketData& BuildingSocketData, EBuildType BuildType);

	 virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
