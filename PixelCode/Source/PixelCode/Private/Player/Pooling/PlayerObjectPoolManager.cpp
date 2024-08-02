// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Pooling/PlayerObjectPoolManager.h"

// Sets default values
APlayerObjectPoolManager::APlayerObjectPoolManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerObjectPoolManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerObjectPoolManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

