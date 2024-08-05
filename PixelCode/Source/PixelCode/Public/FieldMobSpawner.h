// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FieldMobSpawner.generated.h"

UCLASS()
class PIXELCODE_API AFieldMobSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFieldMobSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UBoxComponent* spawnerComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	int32 maxSpawn = 5;

	int32 totalSpawnCount = 0;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float spawnArea = 1000;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float spawnRate = 5.0f;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class AGrux> grux1;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class AGrux> grux2;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class AGrux> grux3;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class ADogBart> dogBart;

	void SpawnMob();

	float currentTime;

	UFUNCTION(Server, Reliable)
	void ServerRPC_Spawn();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Spawn();



	UPROPERTY()
	TArray<AActor*> SpawnedActors;








};
