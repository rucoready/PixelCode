// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/inventory/InventoryComponent.h"
#include "Player/inventory/ItemBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
		// ...
	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent GetIsReplicated() : %s"), GetIsReplicated() ? TEXT("TRUE") : TEXT("FALSE"));
	if (GetIsReplicated() != true)
	{

		SetIsReplicated(true);
	}

	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent GetIsReplicated() : %s"), GetIsReplicated() ? TEXT("TRUE") : TEXT("FALSE"));
	if (GetIsReplicated() != true)
	{

		SetIsReplicated(true);
	}
}


void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const
{
	if (ItemIn) // 포인터가 유효한지 확인
	{
		if (InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextItemByID(UItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn)) // 키로 항목찾기, 작동하면 요소유형의 포인터임.
		{
			return *Result;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* ItemIn) const
{
	if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByPredicate([&ItemIn] // 이중포인터
	(const UItemBase* InventoryItem)
		{

			return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack(); // ID가 같고 풀로차있지않은지
		}
	))
	{
		return *Result;
	}
	return nullptr;
}

int32 UInventoryComponent::CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight()); // 아이템중량에서 총중량뺀값 중량용량이 50이고 20을운반하면 30이됨, 부동소수점이 나오면 가장가까운정수로 내림함.
	if (WeightMaxAddAmount >= RequestedAddAmount) // 최대중량이 현재중량보다 크거나같다면 요청한 숫자반환
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->NumericData.MaxStackSize - StackableItem->Quantity; // 데이터를 가져오고 최대스택크기를 가져와서 현재수량을 뺌

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack); // 둘중 더 작은 값 반환
}

void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);// 단일항목 제거
	OnInventoryUpdated.Broadcast();

}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity); // 항목의 양 제거

	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove); // 수량설정, 항목을 수량으로표시, 제거할 실제양을 뺀 다음 원하는 양을 제거해서 무게조정

	InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();

	OnInventoryUpdated.Broadcast();// 업데이트하고 

	return ActualAmountToRemove; // 실제로 제거한 양 반환
}

void UInventoryComponent::SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit)
{
	if (!(InventoryContents.Num() + 1 > InventorySlotsCapacity)) // 인벤토리에 넘치지않으면 분할
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

void UInventoryComponent::RefreshInventory()
{
	OnInventoryUpdated.Broadcast();
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase* InputItem)
{
	// 유효한 가중치가 있는지 확인, 용량이없을경우
	if (FMath::IsNearlyZero(InputItem->GetItemSingleWeight()) || InputItem->GetItemSingleWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. Item has invalid weight value."), InputItem->TextData.Name)); // 추가하지않음
	}

	// will the item weight overflow weight capacity 추가하면 용량초과
	if (InventoryTotalWeight + InputItem->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. Item would overflow weight limit."), InputItem->TextData.Name));
	}

	// adding one more item would overflow slot capacity 항목을 하나 더 추가하면 슬롯용량초과
	if (InventoryContents.Num() + 1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. All inventory slots are full."), InputItem->TextData.Name));
	}

	// 다 통과했으면 성공
	AddNewItem(InputItem, 1);

	return FItemAddResult::AddedAll(1, FText::Format(FText::FromString("Successfully added a single {0} to the inventory."), InputItem->TextData.Name));
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	return 0;
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;
		// handle non-stackable items
		if (!InputItem->NumericData.bisStackable)
		{
			return HandleNonStackableItems(InputItem);
		}

		// handle stakckable 값 추가
		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

		if (StackableAmountAdded == InitialRequestedAddAmount)
		{
			return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(FText::FromString("Successfully added {0} {1} to the inventory."), InitialRequestedAddAmount, InputItem->TextData.Name));
		}

		if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(FText::FromString("Partial amount of {0} added to the inventory. Number added = {1}"), InputItem->TextData.Name, StackableAmountAdded));
		}

		if (StackableAmountAdded <= 0) // 추가된 값이 0보다 작으면 안되지만
		{
			return FItemAddResult::AddedNone(FText::Format(FText::FromString("Couldn't add {0} to the inventory. No remaining inventory slots, or invalid item."), InputItem->TextData.Name));
		}
	}

	check(false);
	return FItemAddResult::AddedNone(FText::FromString("TryAddItem fallthrough error. GetOwner() check somehow failed."));
}

void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
	UItemBase* NewItem;

	if (Item->bIsCopy || Item->bIsPickup)
	{
		// 항목이 이미 사본이거나 픽업인 경우
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		// 그렇지않은경우
		NewItem = Item->CreateItemCopy(); // 항목 복사본을 생성하고 호출
	}

	NewItem->OwningInventory = this; // 인벤토리에 아이템을 집어넣음
	NewItem->SetQuantity(AmountToAdd);	// 추가할 수량설정

	InventoryContents.Add(NewItem);	// 새 항목 추가
	InventoryTotalWeight += NewItem->GetItemStackWeight();// 가중치 조정
	OnInventoryUpdated.Broadcast(); // 모든 UI요소에 변경사항 적용
}

