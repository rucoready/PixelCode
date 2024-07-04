// Fill out your copyright notice in the Description page of Project Settings.


#include "BossSword.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "BossCollisionComponent.h"

// Sets default values
ABossSword::ABossSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	swordComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("swordComp"));
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> bossOBJ(TEXT("/Script/Engine.StaticMesh'/Game/KMS_AI/Boss_Alpernia/BossSword/BossSword_Sword_low.BossSword_Sword_low'"));
	if (bossOBJ.Succeeded())
	{
		swordComp->SetStaticMesh(bossOBJ.Object);
	}

	SetRootComponent(sceneComp);
	swordComp->SetupAttachment(sceneComp);

	swordComp->SetWorldScale3D(FVector(0.7, 0.6, 1.0));

	bossCollisionComponent = CreateDefaultSubobject<UBossCollisionComponent>(TEXT("CollisionComponent"));

}

// Called when the game starts or when spawned
void ABossSword::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void ABossSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossSword::OnEquipped()
{
	
}

