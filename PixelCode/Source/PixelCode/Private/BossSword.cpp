// Fill out your copyright notice in the Description page of Project Settings.


#include "BossSword.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetStringLibrary.h"

// Sets default values
ABossSword::ABossSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	swordComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("swordComp"));
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	damageSphereComp = CreateDefaultSubobject<UBoxComponent>(TEXT("damageSphereComp"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> bossOBJ(TEXT("/Script/Engine.StaticMesh'/Game/KMS_AI/Boss_Alpernia/BossSword/BossSword_Sword_low.BossSword_Sword_low'"));
	if (bossOBJ.Succeeded())
	{
		swordComp->SetStaticMesh(bossOBJ.Object);
	}

	SetRootComponent(sceneComp);
	swordComp->SetupAttachment(RootComponent);
	damageSphereComp->SetupAttachment(RootComponent);
	damageSphereComp->SetRelativeLocation(FVector(0, 340, 10));
	damageSphereComp->SetRelativeRotation(FRotator(0, 0, 90));
	damageSphereComp->SetWorldScale3D(FVector(1.12, 0.46, 5.85));


	swordComp->SetWorldScale3D(FVector(0.7, 0.6, 1.0));

	bossCollisionComponent = CreateDefaultSubobject<UBossCollisionComponent>(TEXT("CollisionComponent"));

	
	

}

// Called when the game starts or when spawned
void ABossSword::BeginPlay()
{
	Super::BeginPlay();

	damageSphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABossSword::OnBeginOverlapSwordCollision);
	//damageSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//damageSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}

// Called every frame
void ABossSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString CollisionString = UKismetStringLibrary::Conv_IntToString(damageSphereComp->GetCollisionEnabled());

	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Emerald, CollisionString);

}

void ABossSword::OnEquipped()
{
	
}

void ABossSword::Testing1()
{
	UE_LOG(LogTemp, Warning, TEXT("Testing"));
}

void ABossSword::OnBeginOverlapSwordCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains("Player"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap  Sword"));
	}
	
}

void ABossSword::SwordCollisionActive()
{
	//if(IsValid(damageSphereComp))	{ UE_LOG(LogTemp, Warning, TEXT("Sword Collision Activate")); }
	UE_LOG(LogTemp, Warning, TEXT("Sword Collision Activate"));
	damageSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
}

void ABossSword::SwordCollisionDeactive()
{
	//if (IsValid(damageSphereComp)) { UE_LOG(LogTemp, Warning, TEXT("Sword Collision DeActivate")); }
	damageSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	UE_LOG(LogTemp, Warning, TEXT("Sword Collision DeActivate"));
	
	
}



