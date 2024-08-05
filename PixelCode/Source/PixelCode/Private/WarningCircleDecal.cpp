// Fill out your copyright notice in the Description page of Project Settings.


#include "WarningCircleDecal.h"
#include "Components/DecalComponent.h"
#include "Math/UnrealMathUtility.h"
#include "DecalSpawnSword.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Player/PixelCodeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"

AWarningCircleDecal::AWarningCircleDecal()
{
	// 데칼 컴포넌트 초기화 및 설정
	decalComponentOut = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponentOut"));
	decalComponentIn = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponentIn"));

	sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComponent"));
	// 데칼 컴포넌트를 루트 컴포넌트로 설정
	SetRootComponent(sceneComponent);
	decalComponentOut->SetupAttachment(sceneComponent);
	decalComponentIn->SetupAttachment(sceneComponent);

	decalComponentOut->SetRelativeRotation(FRotator(-90,0,0));
	decalComponentIn->SetRelativeRotation(FRotator(-90, 0, 0));
	decalComponentIn->SetWorldScale3D(FVector(0.1f));
	PrimaryActorTick.bCanEverTick = true;
	scaleSpeed = 0.3f; 
	onceExplosion = false;


	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	sphereComp->SetupAttachment(RootComponent);
	sphereComp->SetWorldScale3D(FVector(7.5));
	
	
	
}

void AWarningCircleDecal::BeginPlay()
{
	Super::BeginPlay();
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AWarningCircleDecal::OnBeginOverlapCollision);
	sphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWarningCircleDecal::OnBeginOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains("Player"))
	{
		// OtherActor를 PixelCodeCharacter로 캐스팅
		APixelCodeCharacter* playerCharacter = Cast<APixelCodeCharacter>(OtherActor);
		if (playerCharacter)
		{
			

			FVector sphereLocation = sphereComp->GetComponentLocation();
			FVector playerLocation = playerCharacter->GetActorLocation();
			FVector direction = playerLocation - sphereLocation;
			direction.Normalize();
			float launchForce = 1200.0f;

			playerCharacter->LaunchCharacter(direction * launchForce, true, true);
			playerCharacter->GetHit(direction,true);
			ApplyDamageToTarget(OtherActor, 50);

		}
	}

	
}

void AWarningCircleDecal::ApplyDamageToTarget(AActor* OtherActor, float DamageAmount)
{
	if (IsValid(OtherActor))
	{
		Pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, Pc, Player, UDamageType::StaticClass());
	}
}

void AWarningCircleDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (currentTime > 0.0f && currentTime < 3.0f)
	{
		float normalizedTime = FMath::Clamp(currentTime / 5.0f, 0.0f, 1.0f);
		float scaleFactor = FMath::Lerp(0.05f, 5.0f, normalizedTime* scaleSpeed); 
		decalComponentIn->SetRelativeScale3D(FVector(scaleFactor, scaleFactor, scaleFactor));
	}

	if (currentTime >= 2.3 && !onceSpawnSword)
	{
		if (decalSword)
		{

			AActor* spawnedSword = GetWorld()->SpawnActor<AActor>(decalSword, GetActorLocation()+FVector(0,0,500), GetActorRotation());
			

			onceSpawnSword = true;

		}
	}
	if (currentTime >= 2.7&&!onceExplosion)
	{
		if (decalSword)
		{
			onceExplosion = true;
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), explosionSword, GetActorLocation(), GetActorRotation(), FVector(1.0f));
			sphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			int32 hitValue = FMath::RandRange(1, 5);
			{
				if (hitValue == 1)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), hitImpactSound, GetActorLocation());
				}
				else if (hitValue == 2)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), hitImpactSound2, GetActorLocation());
				}
				else if (hitValue == 3)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), hitImpactSound3, GetActorLocation());
				}
				else if (hitValue == 4)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), hitImpactSound4, GetActorLocation());
				}
				else
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), hitImpactSound5, GetActorLocation());
				}
			}

			
			Destroy();

		}
	}
	

	currentTime += DeltaTime;
}
