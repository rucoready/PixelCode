// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/DataTable.h>
#include "PCodeCharacterCustum.generated.h"

class UDataTable;

UENUM(BlueprintType)
enum class ECharacterTypes : uint8	// ������ ����, ����, ���� , ����Ʈ������ ��
{
	Expendable,
	Equipmentable,
	CharacterAsset UMETA(DisplayName = "Asset"),
	Color UMETA(DisplayName = "Color")

};

USTRUCT()
struct  FItemTextDatas // �ؽ��� ������
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	FText InteractionText;

	UPROPERTY(EditAnywhere)
	FText UsageText;

};

USTRUCT()
struct FItemAssetDatas // ���� ������, ������ �������� ���尰���� �ֱ⵵ ����
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	USkeletalMesh* SkeletalMesh;

};

USTRUCT()
struct  FCharacters : public FTableRowBase // �� ���̽� ���
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	ECharacterTypes CharacterTypes;


	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextDatas TextData;


	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetDatas AssetData;

};


UCLASS()
class PIXELCODE_API APCodeCharacterCustum : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APCodeCharacterCustum();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
