// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalCollision.h"
#include "Components/BoxComponent.h"

// Sets default values
APortalCollision::APortalCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("boxCollision"));

}

// Called when the game starts or when spawned
void APortalCollision::BeginPlay()
{
	Super::BeginPlay();
	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &APortalCollision::OnBeginOverlapPortal);
}

// Called every frame
void APortalCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortalCollision::OnBeginOverlapPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains("Player"))
	{
		
		if (HasAuthority())
		{
			GetWorld()->ServerTravel(TEXT("/Game/KMS_AI/BossMap/Dungeon2?listen"));
		}
		
	}
}

