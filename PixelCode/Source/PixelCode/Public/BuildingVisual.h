#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataTypes.h"
#include "BuildingVisual.generated.h"

class ABuilding;
class UMaterialInstance;
class UItemBase;
class APickup;
class UInventoryComponent;



UCLASS()
class PIXELCODE_API ABuildingVisual : public AActor
{
	GENERATED_BODY()
	
	FORCEINLINE UItemBase* GetItemData() { return ItemReference; };

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

	UPROPERTY(Replicated,EditDefaultsOnly, BlueprintReadWrite, Category = KSH)
	UMaterialInstance* MaterialFalse;

	UPROPERTY(Replicated,EditDefaultsOnly, BlueprintReadWrite, Category = KSH)
	UMaterialInstance* MaterialTrue;

	UPROPERTY(Replicated, BlueprintReadWrite, EditDefaultsOnly,  Category = KSH)
	bool bMaterialIsTrue;

	FVector Loc;
	FVector NewLoc;


	ABuilding* GetHitBuildingActor(const FHitResult& HitResult);

	UPROPERTY(Replicated)
	ABuilding* InteractingBuilding;

	UPROPERTY(EditDefaultsOnly, Category = KSH)
	UItemBase* ItemReference;

	UPROPERTY(Replicated)
	FBuildingSocketData SocketData;

	class APixelCodeCharacter* pc;

	//-------------------------------------------------------------------------------
	void  SetMeshTo(EBuildType BuildType);


	//-------------------------------------------------------------------------------

	void ReturnMeshToSelected();

	//-------------------------------------------------------------------------------

	UPROPERTY(Replicated)
	bool bReturnedMesh;

public:	 
	void SetBuildPosition(const FHitResult& HitResult);

	void SpawnBuilding();

	void DestroyInstance(const FHitResult& HitResult);
	void CycleMesh();

	UPROPERTY()
	bool bItemQuantityValid;
};
