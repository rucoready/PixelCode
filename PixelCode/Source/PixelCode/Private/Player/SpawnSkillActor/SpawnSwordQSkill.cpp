// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SpawnSkillActor/SpawnSwordQSkill.h"
#include "Components/BoxComponent.h"

// Sets default values
ASpawnSwordQSkill::ASpawnSwordQSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void ASpawnSwordQSkill::BeginPlay()
{
	Super::BeginPlay();
	
    // 초기 위치 설정
    SetActorLocation(GetActorLocation() + FVector(0.f, 0.f, 100.f)); // 예시로 액터를 Z축으로 100만큼 이동시킴

}

// Called every frame
void ASpawnSwordQSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector NewLocation = GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime; // Speed는 앞으로 이동할 속도
    SetActorLocation(NewLocation);

}


