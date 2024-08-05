// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFloor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "Player/PixelCodeCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h>
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStatics.h"
#include "DemonSword.h"  
#include "DrawDebugHelpers.h"  
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"

// Sets default values
ABossFloor::ABossFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	floorComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("floorComp"));
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> bossOBJ(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (bossOBJ.Succeeded())
	{
		floorComp->SetStaticMesh(bossOBJ.Object);
	}

	SetRootComponent(sceneComp);
	floorComp->SetupAttachment(RootComponent);
	floorComp->SetWorldScale3D(FVector(120.0f, 120.0f, 1.0f));

    // Ÿ�̸Ӹ� �ʱ�ȭ�մϴ�.
    spawnTimerInterval = 20.0f; 
    spawnDelay = 1.0f; 
}

// Called when the game starts or when spawned
void ABossFloor::BeginPlay()
{
    Super::BeginPlay();
    //DestroyAllSword();
}

// Called every frame
void ABossFloor::Tick(float DeltaTime)
{
    
	Super::Tick(DeltaTime);

    currentTime += DeltaTime;
    
    if (currentTime >= 30.0 && !bOnceActive)
    {
        SpawnSword();
        currentTime = 0.0f;
        bOnceActive = true;
        
    }
    if (currentTime >= 30.0 && bOnceActive)
    {
        bOnceActive = false;
    }

}

void ABossFloor::SpawnSword()
{
    
    for (int32 i = 0; i < 3; ++i)
    {
        
        FVector origin = GetActorLocation();  
        float radius = 3000.0f;  
        FVector ranLoc = origin + FVector(FMath::RandRange(-radius, radius), FMath::RandRange(-radius, radius), 0.0f);

     
        //DrawDebugSphere(GetWorld(), ranLoc, 100.0f, 24, FColor::Green, true);

       
        if (demonSwordFactory)
        {
            AActor* spawnedSword = GetWorld()->SpawnActor<AActor>(demonSwordFactory, ranLoc, FRotator::ZeroRotator);

        }
    }
}
//float DelayTime = i * 0.3f;



//GetWorld()->GetTimerManager().SetTimer(timerhandle_SpawnSword, this, &ABossFloor::TimerSpawnSword, DelayTime, false);


void ABossFloor::DestroyAllSword()
{
    demonSwordFactory = nullptr;
}

