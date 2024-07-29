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

    static ConstructorHelpers::FObjectFinder<UStaticMesh> swordOBJ(TEXT("/Game/KMS_AI/Boss_Alpernia/BossSword/BossSword_Sword_low.BossSword_Sword_low"));
    if (swordOBJ.Succeeded())
    {
        swordComp->SetStaticMesh(swordOBJ.Object);
    }

    
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

    float MoveStartTime = 0.0f;
    float MoveEndTime = 3.0f;

    if (currentTime > MoveStartTime && currentTime < MoveEndTime)
    {
        FVector StartLocation = GetActorLocation();
        FVector TargetLocation = StartLocation + FVector(0.0f, 0.0f, -85.0f);
        float LerpAlpha = FMath::Clamp((currentTime - MoveStartTime) / (MoveEndTime - MoveStartTime), 0.0f, 1.0f);
        FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, LerpAlpha);
        SetActorLocation(NewLocation);
    }

    
	
}

