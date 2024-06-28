// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossFirstPositionActor.h"
#include "Components/BoxComponent.h"

// Sets default values
ABossFirstPositionActor::ABossFirstPositionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	startPositionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("startPositionComp"));
	SetRootComponent(startPositionComp);
}

// Called when the game starts or when spawned
void ABossFirstPositionActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossFirstPositionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

