// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SpawnSkillActor/APlayerMageZSkillSpawnActor.h"

// Sets default values
AAPlayerMageZSkillSpawnActor::AAPlayerMageZSkillSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAPlayerMageZSkillSpawnActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAPlayerMageZSkillSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AAPlayerMageZSkillSpawnActor::OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

