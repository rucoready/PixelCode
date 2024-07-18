// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingArea.h"
#include "CreateItemData.h"
#include "Components/SphereComponent.h"
#include "Player/PixelCodeCharacter.h"
#include "Kismet/GamePlayStatics.h"

// Sets default values
ACraftingArea::ACraftingArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;


	SphereOverlap = CreateDefaultSubobject<USphereComponent>(TEXT("SphereOverlap"));
	SphereOverlap->SetupAttachment(GetRootComponent());

	SphereOverlap->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereOverlap->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	CraftArea = ECraftArea::ECA_MAX;
}

// Called when the game starts or when spawned
void ACraftingArea::BeginPlay()
{
	Super::BeginPlay();

	SphereOverlap->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACraftingArea::OnSphereBeginOverlap);
	
	SphereOverlap->OnComponentEndOverlap.AddUniqueDynamic(this, &ACraftingArea::OnSphereEndOverlap);
	
}

void ACraftingArea::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APixelCodeCharacter* Char = Cast<APixelCodeCharacter>(OtherActor);
	if(Char)
	{
		Char->AddCraftArea(CraftArea);
	}
}

void ACraftingArea::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex)
{
	APixelCodeCharacter* Char = Cast<APixelCodeCharacter>(OtherActor);
	if (Char)
	{
		Char->RemoveArea(CraftArea);
	}
}


