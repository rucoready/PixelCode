// Fill out your copyright notice in the Description page of Project Settings.


#include "FractureDragonStatue.h"
#include "GeometryCollection/GeometryCollectionObject.h"

// Sets default values
AFractureDragonStatue::AFractureDragonStatue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

// 	fractureStatue = CreateDefaultSubobject<UGeometryCollection>(TEXT("fractureStatue"));
// 	

}

// Called when the game starts or when spawned
void AFractureDragonStatue::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFractureDragonStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

