// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldMobSpawner.h"
#include "Components/BoxComponent.h"
#include "Grux.h"
#include "DogBart.h"
#include "EngineUtils.h"

// Sets default values
AFieldMobSpawner::AFieldMobSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	spawnerComp = CreateDefaultSubobject<UBoxComponent>(TEXT("spawnerComp"));
	SetRootComponent(spawnerComp);

}

// Called when the game starts or when spawned
void AFieldMobSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFieldMobSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    currentTime += DeltaTime;
    SpawnMob();
}

void AFieldMobSpawner::SpawnMob()
{
    
    // 타이머가 설정된 스폰 주기에 도달했는지 확인합니다.
    if (currentTime >= spawnRate)
    {
        currentTime = 0.0f; // 타이머 초기화

        FVector spawnLocation = spawnerComp->GetComponentLocation();
        FVector spawnExtent = FVector(spawnArea, spawnArea, 0.0f);

        // 현재 스폰된 액터의 개수를 카운트합니다.
        int32 currentSpawnCount = 0;

        // 스폰할 액터 클래스들을 배열에 넣습니다.
        TArray<TSubclassOf<AActor>> spawnClasses = { grux1, grux2, dogBart };

        // 현재 스폰된 액터의 개수를 계산합니다.
        for (TSubclassOf<AActor> spawnClass : spawnClasses)
        {
            // 현재 클래스로 스폰된 액터의 수를 센다.
            for (TActorIterator<AActor> ActorItr(GetWorld(), spawnClass); ActorItr; ++ActorItr)
            {
                currentSpawnCount++;
            }
        }

        // 최대 스폰 개수(maxSpawn)에 도달할 때까지 스폰합니다.
        while (currentSpawnCount < maxSpawn)
        {
            // 무작위로 선택된 액터 클래스를 정합니다.
            TSubclassOf<AActor> randomSelectedMob = spawnClasses[FMath::RandRange(0, spawnClasses.Num() - 1)];

            // 스폰 위치를 랜덤하게 지정합니다.
            FVector spawnLocationOffset = spawnLocation + FVector(FMath::RandRange(-spawnExtent.X, spawnExtent.X), FMath::RandRange(-spawnExtent.Y, spawnExtent.Y), 0.0f);

            // 액터를 스폰합니다.
            AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(randomSelectedMob, spawnLocationOffset, FRotator::ZeroRotator);

            // 스폰된 액터 개수를 증가시킵니다.
            currentSpawnCount++;
        }
    }
}

