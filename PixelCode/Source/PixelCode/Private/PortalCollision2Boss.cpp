// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalCollision2Boss.h"
#include "Components/BoxComponent.h"

// Sets default values
APortalCollision2Boss::APortalCollision2Boss()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("boxCollision"));

}

// Called when the game starts or when spawned
void APortalCollision2Boss::BeginPlay()
{
	Super::BeginPlay();
	
	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &APortalCollision2Boss::OnBeginOverlapPortal);
}

// Called every frame
void APortalCollision2Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortalCollision2Boss::OnBeginOverlapPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains("Player"))
	{

		if (HasAuthority())
		{
			GetWorld()->ServerTravel(TEXT("/Game/KMS_AI/BossMap/BossMap2?Listen"));
			
		}

	}
}

