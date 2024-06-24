// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitActor.h"

// Sets default values
AUnitActor::AUnitActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUnitActor::BeginPlay()
{
	Super::BeginPlay();
	FString s = FString(" Wood ");
	Cur_UnitData = UnitDataTable->FindRow<FUnitActorData>(FName("Wood"), s,true);
	
}

// Called every frame
void AUnitActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Cur_UnitData)
	{
		GEngine->AddOnScreenDebugMessage(-1, 50.f,FColor::Red, FString::FromInt(Cur_UnitData->itemNum));
	}
}

