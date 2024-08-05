// Fill out your copyright notice in the Description page of Project Settings.


#include "DragonRazorStatue.h"
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
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "CoreMinimal.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "PCodePlayerController.h"
#include "FractureDragonStatue.h"
#include "Boss/BossApernia.h"
#include "Boss/BossAIController.h"
#include "EngineUtils.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Player/pixelPlayerState.h"

// Sets default values
ADragonRazorStatue::ADragonRazorStatue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


    statueComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("orbMesh"));
    sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));

    razorComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("razorComponent"));
    fireComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("fireComponent"));


    static ConstructorHelpers::FObjectFinder<UStaticMesh> statueOBJ(TEXT("/Script/Engine.StaticMesh'/Game/KMS_AI/DragonStatue/DragonStatue.DragonStatue'"));
    if (statueOBJ.Succeeded())
    {
        statueComp->SetStaticMesh(statueOBJ.Object);
    }

    // Set sphereComp as the root component
    SetRootComponent(statueComp);

    // Attach expComp to sphereComp
    sphereComp->SetupAttachment(RootComponent);
    razorComponent->SetupAttachment(RootComponent);
    fireComponent->SetupAttachment(RootComponent);
    // Optionally set the scale of expComp and sphereComp
    sphereComp->SetWorldScale3D(FVector(12.0));
    sphereComp->SetRelativeLocation(FVector(0,0,276.4942));


}

// Called when the game starts or when spawned
void ADragonRazorStatue::BeginPlay()
{
	Super::BeginPlay();

    currentHp = maxHp;
	
}

// Called every frame
void ADragonRazorStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (currentHp <= 0.0f&&!onceDie)
    {
        onceDie = true;
        
        SpawnFractureStatue();
        GetWorldTimerManager().SetTimer(timerhandle_Destroy, this, &ADragonRazorStatue::DrestroyFractureStatue, 5.0f, false);

        DestroySelf();

        UGameplayStatics::PlaySoundAtLocation(GetWorld(), fractureSound, GetActorLocation());
        
    }

    

}

void ADragonRazorStatue::TakeDamageD(float damage)
{
    currentHp = currentHp - damage;

    int32 MaterialIndex = 0; // 적절한 슬롯 인덱스 지정
    
    statueComp->SetMaterial(MaterialIndex, damagelMaterial);
    
    GetWorldTimerManager().SetTimer(timerhandle_SetOriginMaterial, this, &ADragonRazorStatue::RestoreOriginMaterial, 0.25f, false);
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), fractureHitSound, GetActorLocation());
}

void ADragonRazorStatue::OnMyClickBlackRoom()
{
}

void ADragonRazorStatue::SpawnFractureStatue()
{
    if (fractureStatue)
    {
        fracturStatue = GetWorld()->SpawnActor<AActor>(fractureStatue, GetActorLocation(),GetActorRotation());
        
        
    }
}

void ADragonRazorStatue::DestroySelf()
{
    
    ServerRPC_DestroyCheck();
}

void ADragonRazorStatue::DrestroyFractureStatue()
{
    
    if (fractureStatue)
    {
        
        fracturStatue->Destroy();
        
        
		
        
    }

}

void ADragonRazorStatue::RestoreOriginMaterial()
{
    if (statueComp)
    {
        int32 MaterialIndex2 = 0; // 적절한 슬롯 인덱스 지정
        statueComp->SetMaterial(MaterialIndex2, originalMaterial);
    }
    
}

void ADragonRazorStatue::ServerRPC_DestroyCheck_Implementation()
{
    MulticastRPC_DestroyCheck();
}

void ADragonRazorStatue::MulticastRPC_DestroyCheck_Implementation()
{
    statueComp->DestroyComponent(true);
    razorComponent->SetVisibility(false);
    fireComponent->SetVisibility(false);
    for (TActorIterator<ABossAIController> It(GetWorld()); It; ++It)
    {
        ABossAIController* BossAIController = *It;
        if (BossAIController)
        {
            if (ABossApernia* boss = Cast<ABossApernia>(BossAIController->GetPawn()))
            {
                boss->statueDestroyCount += 1;
            }
        }

    }
}



