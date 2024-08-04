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
    
    // Ÿ�̸Ӱ� ������ ���� �ֱ⿡ �����ߴ��� Ȯ���մϴ�.
    if (currentTime >= spawnRate)
    {
        currentTime = 0.0f; // Ÿ�̸� �ʱ�ȭ

        FVector spawnLocation = spawnerComp->GetComponentLocation();
        FVector spawnExtent = FVector(spawnArea, spawnArea, 0.0f);

        // ���� ������ ������ ������ ī��Ʈ�մϴ�.
        int32 currentSpawnCount = 0;

        // ������ ���� Ŭ�������� �迭�� �ֽ��ϴ�.
        TArray<TSubclassOf<AActor>> spawnClasses = { grux1, grux2, dogBart };

        // ���� ������ ������ ������ ����մϴ�.
        for (TSubclassOf<AActor> spawnClass : spawnClasses)
        {
            // ���� Ŭ������ ������ ������ ���� ����.
            for (TActorIterator<AActor> ActorItr(GetWorld(), spawnClass); ActorItr; ++ActorItr)
            {
                currentSpawnCount++;
            }
        }

        // �ִ� ���� ����(maxSpawn)�� ������ ������ �����մϴ�.
        while (currentSpawnCount < maxSpawn)
        {
            // �������� ���õ� ���� Ŭ������ ���մϴ�.
            TSubclassOf<AActor> randomSelectedMob = spawnClasses[FMath::RandRange(0, spawnClasses.Num() - 1)];

            // ���� ��ġ�� �����ϰ� �����մϴ�.
            FVector spawnLocationOffset = spawnLocation + FVector(FMath::RandRange(-spawnExtent.X, spawnExtent.X), FMath::RandRange(-spawnExtent.Y, spawnExtent.Y), 0.0f);

            // ���͸� �����մϴ�.
            AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(randomSelectedMob, spawnLocationOffset, FRotator::ZeroRotator);

            // ������ ���� ������ ������ŵ�ϴ�.
            currentSpawnCount++;
        }
    }
}

