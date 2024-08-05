// ADestroyAllActor.cpp

#include "DestroyAllActor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundBase.h" 
#include "Player/PlayerOrganism.h"

ADestroyAllActor::ADestroyAllActor()
{
	PrimaryActorTick.bCanEverTick = true;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	damageBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("damageBoxComp"));

	SetRootComponent(sceneComp);
	damageBoxComp->SetupAttachment(RootComponent);
	activeCollisionOnce = false;

	for (int i = 0; i < 40; ++i)
	{
		overlapOnce[i] = false;
	}
}

void ADestroyAllActor::BeginPlay()
{
	Super::BeginPlay();

	damageBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ADestroyAllActor::OnBeginOverlapCollision);

	currentTime = 0.0f;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), swordDestroyNiagara, GetActorLocation(), GetActorRotation(), FVector(3.0f));
	damageBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADestroyAllActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentTime += DeltaTime;

	if (currentTime > 9.0f && !bAppliedInitialDamage)
	{
		if (IsValid(Player))
		{
			ApplyDamageToTarget(Player, 8);
			bAppliedInitialDamage = true;
		}
	}

	if (currentTime > 5.0f && !bCollisionEnabled)
	{
		damageBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		bCollisionEnabled = true;
	}

	if (currentTime > 5.0f && currentTime <= 9.9f)
	{
		for (int i = 0; i < 40; ++i)
		{
			float overlapTime = 4.0f + i * 0.1f;
			if (currentTime >= overlapTime && !overlapOnce[i])
			{
				if (IsValid(Player))
				{
					ApplyDamageToTarget(Player, 8);
					overlapOnce[i] = true;
				}
			}
		}
	}

	if (currentTime > 11.9f && !overlapOnce[39])
	{
		if (IsValid(Player))
		{
			ApplyDamageToTarget(Player, 500);
			overlapOnce[39] = true;
		}
	}
	if (currentTime > 13.0f)
	{
		Destroy();
	}
}

void ADestroyAllActor::ApplyDamageToTarget(AActor* OtherActor, float DamageAmount)
{
	if (IsValid(OtherActor))
	{
		Pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, Pc, Player, UDamageType::StaticClass());

		PlayDamageSound();
	}
}

void ADestroyAllActor::PlayDamageSound()
{
	int32 damageValue = FMath::RandRange(1, 7);

	if (damageValue == 1)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), damageSound1, GetActorLocation());
	}
	else if (damageValue == 2)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), damageSound2, GetActorLocation());
	}
	else if (damageValue == 3)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), damageSound3, GetActorLocation());
	}
	else if (damageValue == 4)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), damageSound4, GetActorLocation());
	}
	else if (damageValue == 5)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), damageSound5, GetActorLocation());
	}
	else if (damageValue == 6)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), damageSound6, GetActorLocation());
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), damageSound7, GetActorLocation());
	}
	
}

void ADestroyAllActor::OnBeginOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->GetName().Contains("Player"))
	{
		Player = Cast<APlayerOrganism>(OtherActor);
		if (Player)
		{
			Player->GetHit(SweepResult.ImpactPoint, false);
		}
		activeCollisionOnce = true;
		ApplyDamageToTarget(OtherActor, 12);
	}
}

void ADestroyAllActor::StartPeriodicDamageApplication(AActor* OtherActor, float damageAmount)
{
	ApplyDamageToTarget(OtherActor, 12);
}
