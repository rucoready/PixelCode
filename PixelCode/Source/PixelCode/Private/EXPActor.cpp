// Fill out your copyright notice in the Description page of Project Settings.


#include "EXPActor.h"
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
#include "Player/pixelPlayerState.h"
#include "Sound/SoundBase.h" 
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values
AEXPActor::AEXPActor()
{
    PrimaryActorTick.bCanEverTick = true;

    expComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("orbMesh"));
    sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
    RunningTime = 0.0f;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> expOrb(TEXT("/Script/Engine.StaticMesh'/Game/KMS_AI/Exp/SM_Exp.SM_Exp'"));
    if (expOrb.Succeeded())
    {
        expComp->SetStaticMesh(expOrb.Object);
    }

    // Set sphereComp as the root component
    SetRootComponent(expComp);

    // Attach expComp to sphereComp
    sphereComp->SetupAttachment(RootComponent);

    // Optionally set the scale of expComp and sphereComp
    expComp->SetWorldScale3D(FVector(0.1));
    sphereComp->SetWorldScale3D(FVector(1.0));

}

// Called when the game starts or when spawned
void AEXPActor::BeginPlay()
{
	Super::BeginPlay();

	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AEXPActor::OnBeginOverlapExpOrb);


    FVector launchVelocity = FVector(0, 0, 700);
    expComp->AddImpulse(launchVelocity, NAME_None, true);
	
}

// Called every frame
void AEXPActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    // 월드에서 모든 APixelCodeCharacter 찾기
    TArray<AActor*> FoundPlayers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), FoundPlayers);

    for (AActor* Actor : FoundPlayers)
    {
        APixelCodeCharacter* Player = Cast<APixelCodeCharacter>(Actor);
        if (Player)
        {
            float Distance = FVector::Dist(this->GetActorLocation(), Player->GetActorLocation());

            // 거리가 300 이하일 경우 Lerp로 이동
            if (Distance <= 300.0f)
            {
                FVector NewLocation = FMath::VInterpTo(GetActorLocation(), Player->GetActorLocation(), DeltaTime, 5.0f); // 5.0f는 Lerp 속도
                SetActorLocation(NewLocation);
            }
        }
    }
}

void AEXPActor::OnBeginOverlapExpOrb(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	

    if (OtherActor->GetName().Contains("Player"))
    {
        APixelCodeCharacter* Player = Cast<APixelCodeCharacter>(OtherActor);
        if (Player)
        {
            APCodePlayerController* Pc = Cast<APCodePlayerController>(Player->GetController());
            if (Pc && Pc->IsLocalController())
            {
                // 서버에서 클라이언트의 PlayerState를 업데이트하려면 RPC를 사용하는 것이 좋습니다.
                if (Pc->IsLocalController()) // 클라이언트에서만 실행됨
                {
                    ApixelPlayerState* PlayerState = Cast<ApixelPlayerState>(Pc->PlayerState);
                    if (PlayerState)
                    {
                        PlayerState->SetaddUpEXP(50.0f);

                        int32 expSoundValue = FMath::RandRange(1, 7);
                        {
                            if (expSoundValue == 1)
                            {
                                UGameplayStatics::PlaySoundAtLocation(GetWorld(), expSound1, GetActorLocation());
                            }
                            else if (expSoundValue == 2)
                            {
                                UGameplayStatics::PlaySoundAtLocation(GetWorld(), expSound2, GetActorLocation());
                            }
                            else if (expSoundValue == 3)
                            {
                                UGameplayStatics::PlaySoundAtLocation(GetWorld(), expSound3, GetActorLocation());
                            }
                            else if (expSoundValue == 4)
                            {
                                UGameplayStatics::PlaySoundAtLocation(GetWorld(), expSound4, GetActorLocation());
                            }
                            else if (expSoundValue == 5)
                            {
                                UGameplayStatics::PlaySoundAtLocation(GetWorld(), expSound5, GetActorLocation());
                            }
                            else if (expSoundValue == 6)
                            {
                                UGameplayStatics::PlaySoundAtLocation(GetWorld(), expSound6, GetActorLocation());
                            }
                            else
                            {
                                UGameplayStatics::PlaySoundAtLocation(GetWorld(), expSound7, GetActorLocation());
                            }


                        }

                    }
                }
                else
                {
                    // 서버에서 RPC 호출
                    ClientAddExp(Pc, 50.0f);
                }
            }
        }

        Destroy();
    }

}


void AEXPActor::ClientAddExp_Implementation(APCodePlayerController* Pc, float ExpAmount)
{
    if (Pc && Pc->IsLocalController())
    {
        ApixelPlayerState* PlayerState = Cast<ApixelPlayerState>(Pc->PlayerState);
        if (PlayerState)
        {
            PlayerState->SetaddUpEXP(ExpAmount);
        }
    }
}