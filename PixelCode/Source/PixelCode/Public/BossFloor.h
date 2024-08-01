// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DemonSword.h"
#include "BossFloor.generated.h"

UCLASS()
class PIXELCODE_API ABossFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UStaticMeshComponent* floorComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class TSubclassOf<class ADemonSword> demonSwordFactory;

	// Ÿ�̸Ӹ� �ʱ�ȭ�մϴ�.
	float spawnTimerInterval = 20.0f; // 20�� �������� ����
	float spawnDelay = 1.0f; // ���� �����

	void SpawnSword();

	float currentTime;

	FTimerHandle timerhandle_SpawnSword;


	bool bOnceActive = false;

	void DestroyAllSword();

	

	
};
