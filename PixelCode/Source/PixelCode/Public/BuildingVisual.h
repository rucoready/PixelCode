#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataTypes.h"
#include "BuildingVisual.generated.h"

class ABuilding;
class UMaterialInstance;


UCLASS()
class PIXELCODE_API ABuildingVisual : public AActor
{
	GENERATED_BODY()
	
public:	
	ABuildingVisual();


	virtual void BeginPlay() override;

	UPROPERTY(Replicated,EditDefaultsOnly, BlueprintReadOnly, Category = KSH)
	class UStaticMeshComponent* BuildMesh;

	UPROPERTY(Replicated,EditDefaultsOnly, Category = KSH)
	TSubclassOf<ABuilding> BuildingClass;

	UPROPERTY(Replicated,EditDefaultsOnly, Category = KSH)
	TArray<FBuildingVisualType> BuildingTypes;

	UPROPERTY(Replicated)
	uint8 BuildingTypeIndex;

	UPROPERTY(Replicated,EditDefaultsOnly, Category = KSH)
	UMaterialInstance* MaterialFalse;

	UPROPERTY(Replicated,EditDefaultsOnly, Category = KSH)
	UMaterialInstance* MaterialTrue;

	UPROPERTY(Replicated)
	bool bMaterialIsTrue;


	ABuilding* GetHitBuildingActor(const FHitResult& HitResult);

	UPROPERTY(Replicated)
	ABuilding* InteractingBuilding;

	UPROPERTY(Replicated)
	FBuildingSocketData SocketData;

	void  SetMeshTo(EBuildType BuildType);

	void ReturnMeshToSelected();
	
	UPROPERTY(Replicated)
	bool bReturnedMesh;

public:	 
	void SetBuildPosition(const FHitResult& HitResult);
	void SpawnBuilding();
	void DestroyInstance(const FHitResult& HitResult);
	void CycleMesh();

};
