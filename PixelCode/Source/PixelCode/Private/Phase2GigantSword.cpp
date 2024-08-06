// Fill out your copyright notice in the Description page of Project Settings.


#include "Phase2GigantSword.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h>
#include "Player/PlayerOrganism.h"


// Sets default values
APhase2GigantSword::APhase2GigantSword()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    swordComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("swordComp"));
    sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));

    

    
    currentTime = 0.0f;
}

// Called when the game starts or when spawned
void APhase2GigantSword::BeginPlay()
{
	Super::BeginPlay();
    
}

// Called every frame
void APhase2GigantSword::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    currentTime += DeltaTime;

    
    
	
}

