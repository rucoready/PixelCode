// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/ParentItem.h" // ����ü ���� ����� ����
#include "CreateItemData.h"
#include "ItemBase.generated.h"


class UInventoryComponent;

/**
 * 
 */
UCLASS()
class PIXELCODE_API UItemBase : public UObject // object�� C++����
{
	GENERATED_BODY()
	
 public:
	//================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================


	TArray<FCraftItem> GetAllCrafting();

	FCraftItemInfo GetCraftItemInfoBasedOn(EItemName Name);


	UPROPERTY()
	UInventoryComponent* OwningInventory; // �κ��丮


	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 Quantity;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FName ID;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemQuality ItemQuality;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemName ItemName;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemStatistics ItemStatistics;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemTextData TextData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemNumericData NumericData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemAssetData AssetData;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	TMap<EItemName, FCraftItemInfo> CraftItemData; // ũ������Ʈ ������

	UPROPERTY(VisibleAnywhere, Category = "Item")
	TArray<FCraftItem> Crafting;

	bool bIsCopy;
	bool bIsPickup;

	//================================================================================
	// FUNCTIONS
	//================================================================================
	UItemBase();

	void ResetItemFlags(); // �Ⱦ���� �߰�

	UItemBase* CreateItemCopy() const; // ������ ����

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * NumericData.Weight; };// ȣ���� ������带 ũ�� �ٿ���, �� ������

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return Quantity * NumericData.Weight; };// ������ ���� ���Ϲ���

	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullItemStack() const { return Quantity == NumericData.MaxStackSize;};// ��ü �׸� ����, ������ �ִ� ���� ũ��� ������

	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity); // ���� �������� üũ

	UFUNCTION(Category = "Item")
	virtual void Use(APixelCodeCharacter* Character);// �÷��̾� ĳ���� ������ ĳ���͸� ������


protected:
	
	bool operator == (const FName& OtherID) const // TArry �� �����ϰ� ����
	{
		return this->ID == OtherID;
	}
};


