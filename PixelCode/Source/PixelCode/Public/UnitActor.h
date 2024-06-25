// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "UnitActor.generated.h"


UENUM(BlueprintType)
enum class IItemType : uint8
{
    Weapon,		// ����
    Consumable, // �Ҹ�ǰ
	ingredient  // ���
    // �߰����� ������ ������ �ʿ信 ���� ������ �� �ֽ��ϴ�.
};


USTRUCT(BlueprintType)
struct FUnitActorData : public FTableRowBase // ������ ����ü
{
	GENERATED_BODY()
		

	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 itemNum; // ������ ��ȣ
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText name;		// ������ �̸�
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText description; // �����
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString itemType; // ������ id , ������ �̸��� ����, ������ Ÿ�� , �Ӽ�, ���� , ��밡�� ����, Ư�� ȿ��, ������ �̓G
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> Attributes;  // �Ӽ�
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		IItemType uniItemType; // ������ Ÿ��  ���� , �Ҹ�ǰ ,���
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UTexture2D> Image;	// �̹��� 
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* Mesh;		// ����ƽ �Ž�
		
		
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
