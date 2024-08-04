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

	UPROPERTY(Replicated)
	TArray<FName> MeshInstancedSockets;

	virtual void BeginPlay() override;

	bool IsValidSocket(UInstancedStaticMeshComponent* HitComponent, int32 Index, const FName& Filter, const FName& SocketName);

public:

	class APixelCodeCharacter* pc;

	UPROPERTY(Replicated)
	TArray<FInstanceSocketCheck> InstanceSocketsCheck;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = KSH)
	class UInstancedStaticMeshComponent* BaseInstancedMesh;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = KSH)
	class UInstancedStaticMeshComponent* WallInstancedMesh;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = KSH)
	class UInstancedStaticMeshComponent* CeilingInstancedMesh;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = KSH)
	class UInstancedStaticMeshComponent* RoofInstancedMesh;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = KSH)
	class UInstancedStaticMeshComponent* GableInstancedMesh;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = KSH)
	class UInstancedStaticMeshComponent* StairsInstancedMesh;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = KSH)
	class UInstancedStaticMeshComponent* WindowInstancedMesh;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = KSH)
	class UInstancedStaticMeshComponent* ArchInstancedMesh;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = KSH)
	class UInstancedStaticMeshComponent* FloorInstancedMesh;

	UFUNCTION(BlueprintCallable, Category = KSH)
	void DestroyInstance(const FBuildingSocketData& BuildingSocketData);

	UFUNCTION(BlueprintCallable, Category = KSH)
	 FTransform GetInstancedSocketTransform(UInstancedStaticMeshComponent* InstancedComponent, int32 InstanceIndex, const FName& SocketName);

	 int32 GetHitIndex(const FHitResult& HitResult);

	 FBuildingSocketData GetHitSocketTransform(const FHitResult& HitResult, const FName& Filter, float ValidHitDistance = 100.0f);

	 void AddInstance(const FBuildingSocketData& BuildingSocketData, EBuildType BuildType);

	 virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
