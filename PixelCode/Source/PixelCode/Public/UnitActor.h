// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "UnitActor.generated.h"


UENUM(BlueprintType)
enum class IItemType : uint8
{
    Weapon,		// 무기
    Consumable, // 소모품
	ingredient  // 재료
    // 추가적인 아이템 유형은 필요에 따라 정의할 수 있습니다.
};


USTRUCT(BlueprintType)
struct FUnitActorData : public FTableRowBase // 아이템 구조체
{
	GENERATED_BODY()
		

	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 itemNum; // 아이템 번호
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText name;		// 아이템 이름
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText description; // 설명란
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString itemType; // 아이템 id , 아이템 이름과 설명, 아이템 타입 , 속성, 가격 , 사용가능 조건, 특수 효과, 아이템 이밎
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> Attributes;  // 속성
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		IItemType uniItemType; // 아이템 타입  무기 , 소모품 ,재료
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UTexture2D> Image;	// 이미지 
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* Mesh;		// 스태틱 매쉬
		
		
};


UCLASS()
class PIXELCODE_API AUnitActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnitActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UDataTable* UnitDataTable;

	FUnitActorData* Cur_UnitData;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
