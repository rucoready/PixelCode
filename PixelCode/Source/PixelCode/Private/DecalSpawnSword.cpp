// Fill out your copyright notice in the Description page of Project Settings.


#include "DecalSpawnSword.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h>
#include "Player/PlayerOrganism.h"

// Sets default values
ADecalSpawnSword::ADecalSpawnSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	swordComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("swordComp"));
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> swordOBJ(TEXT("/Script/Engine.StaticMesh'/Game/KMS_AI/DecalSword/Death_Sword.Death_Sword'"));
	if (swordOBJ.Succeeded())
	{
		swordComp->SetStaticMesh(swordOBJ.Object);
	}

	SetRootComponent(sceneComp);
	swordComp->SetupAttachment(RootComponent);
	swordComp->SetWorldScale3D(FVector(10.0f));
	swordComp->SetRelativeLocation(FVector(0,0,520));
	swordComp->SetRelativeRotation(FRotator(0,-90,-90));

}

// Called when the game starts or when spawned
void ADecalSpawnSword::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADecalSpawnSword::Tick(float DeltaTime)
{

    Super::Tick(DeltaTime);
    currentTime +=DeltaTime;
    float SpeedFactor1 = 2.0f;  // 1.2초 구간의 이동 속도 배율
    float SpeedFactor2 = 18.0f;  // 4.0초 구간의 이동 속도 배율

    if (currentTime <= 0.8f)
    {
        // 1.2초까지는 midZ 위치로 이동 (SpeedFactor1 적용)
        currentTime += DeltaTime * SpeedFactor1;
        newZ = FMath::Lerp(startZ, midZ, currentTime / 0.8f);
    }
    else if (currentTime <= 4.0f)
    {
        // 1.2초 이후부터 4초까지는 endZ 위치로 이동 (SpeedFactor2 적용)
        currentTime += DeltaTime * SpeedFactor2;
        float lerpFactor = FMath::Clamp((currentTime - 1.2f) / 2.8f, 0.0f, 1.0f);
        newZ = FMath::Lerp(midZ, endZ, lerpFactor);
    }
    else
    {
        // currentTime이 4.0초를 초과한 경우, endZ 위치를 유지
        newZ = endZ;
    }
    if (currentTime >=3.7f)
    {
        Destroy();
    }

    FVector newLocation = swordComp->GetRelativeLocation();
    newLocation.Z = newZ;
    swordComp->SetRelativeLocation(newLocation);
}

