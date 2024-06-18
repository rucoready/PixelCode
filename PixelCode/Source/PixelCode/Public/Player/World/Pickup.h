// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/Interfaces/InteractionInterface.h"
#include "Pickup.generated.h"

class UDataTable;
class UItemBase;
class APlayerOrganism;

UCLASS()
class PIXELCODE_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	//================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================
	

	//================================================================================
	// FUNCTIONS
	//================================================================================
	APickup();

	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity); // 픽업 초기화

	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity); // 드랍 초기화

	FORCEINLINE UItemBase* GetItemData() { return ItemReference; };

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	

protected:
	//================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	UDataTable* ItemDataTable;


	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference; // 픽업이 가리키는 항목

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity; // 수량

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData; // 인스턴스 상호작용가능


	//================================================================================
	// FUNCTIONS
	//================================================================================
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(APixelCodeCharacter* PlayerCharacter) override;
	void UpdateInteractableData();

	void TakePickup(const APlayerOrganism* Taker); // 캐릭터의 인벤토리와 인터페이스해야함

#if WITH_EDITOR // 매크로
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	void SetInput(const APlayerOrganism* Taker);
};
