// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldMobSpawner.h"
#include "Components/BoxComponent.h"
#include "Grux.h"
#include "DogBart.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "EngineUtils.h"

// Sets default values
AFieldMobSpawner::AFieldMobSpawner()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    spawnerComp = CreateDefaultSubobject<UBoxComponent>(TEXT("spawnerComp"));
    SetRootComponent(spawnerComp);

    currentTime = 0.0f;


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
    //FString Timecheck = FString::SanitizeFloat(currentTime);
    //GEngine->AddOnScreenDebugMessage(-1, 0.001f,FColor::White,Timecheck);
    if (currentTime >= spawnRate)
    {
        currentTime = 0.0f;
        ServerRPC_Spawn();
    }
}

void AFieldMobSpawner::SpawnMob()
{

    FVector spawnLocation = spawnerComp->GetComponentLocation();
    FVector spawnExtent = FVector(spawnArea, spawnArea, 0.0f);

    TArray<TSubclassOf<AActor>> spawnClasses = { grux1, grux2, grux3, dogBart };

    SpawnedActors.RemoveAll([](AActor* Actor) { return !Actor || !Actor->IsValidLowLevel(); });

    int32 currentSpawnCount = SpawnedActors.Num();
    //logo


    int32 actorsToSpawn = FMath::Max(0, maxSpawn - currentSpawnCount);

    if (actorsToSpawn > 0)
    {
        int32 spawnCount = FMath::Min(actorsToSpawn, FMath::CeilToInt(spawnRate));

        for (int32 i = 0; i < spawnCount; i++)
        {
            TSubclassOf<AActor> randomSelectedMob = spawnClasses[FMath::RandRange(0, spawnClasses.Num() - 1)];
            FVector spawnLocationOffset = spawnLocation + FVector(FMath::RandRange(-spawnExtent.X, spawnExtent.X), FMath::RandRange(-spawnExtent.Y, spawnExtent.Y), 0.0f);

            AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(randomSelectedMob, spawnLocationOffset, FRotator::ZeroRotator);

            if (spawnedActor)
            {
                SpawnedActors.Add(spawnedActor);

                if (SpawnedActors.Num() >= maxSpawn)
                {
                    break;
                }
            }
        }
    }
}

void AFieldMobSpawner::ServerRPC_Spawn_Implementation()
{
    MulticastRPC_Spawn();
}

void AFieldMobSpawner::MulticastRPC_Spawn_Implementation()
{
    SpawnMob();
}



