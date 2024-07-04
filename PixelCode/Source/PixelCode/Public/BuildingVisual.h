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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = KSH)
	class UStaticMeshComponent* BuildMesh;

	UPROPERTY(EditDefaultsOnly, Category = KSH)
	TSubclassOf< ABuilding> BuildingClass;

	UPROPERTY(EditDefaultsOnly, Category = KSH)
	TArray<FBuildingVisualType> BuildingTypes;

	uint8 BuildingTypeIndex;

	UPROPERTY(EditDefaultsOnly, Category = KSH)
	UMaterialInstance* MaterialFalse;

	UPROPERTY(EditDefaultsOnly, Category = KSH)
	UMaterialInstance* MaterialTrue;

	bool bMaterialIsTrue;

	ABuilding* GetHitBuildingActor(const FHitResult& HitResult);

	ABuilding* InteractingBuilding;

	FBuildingSocketData SocketData;

	void  SetMeshTo(EBuildType BuildType);

	void ReturnMeshToSelected();
	
	bool bReturnedMesh;

public:	 
	void SetBuildPosition(const FHitResult& HitResult);
	void SpawnBuilding();
	void DestroyInstance(const FHitResult& HitResult);
	void CycleMesh();

};
