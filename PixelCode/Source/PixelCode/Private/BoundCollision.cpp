// Fill out your copyright notice in the Description page of Project Settings.


#include "BoundCollision.h"
#include "Components/DecalComponent.h"
#include "Math/UnrealMathUtility.h"
#include "DecalSpawnSword.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Player/PixelCodeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"

// Sets default values
ABoundCollision::ABoundCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	sphereComp->SetupAttachment(RootComponent);
	sphereComp->SetWorldScale3D(FVector(7.5));
}

// Called when the game starts or when spawned
void ABoundCollision::BeginPlay()
{
	Super::BeginPlay();

	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABoundCollision::OnBeginOverlapCollision);
	GetWorldTimerManager().SetTimer(timerhandle_DestroySelf, this, &ABoundCollision::DestroySelf, 0.5f, false);
	
}

// Called every frame
void ABoundCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoundCollision::ApplyDamageToTarget(AActor* OtherActor, float DamageAmount)
{
	if (IsValid(OtherActor))
	{
		Pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, Pc, Player, UDamageType::StaticClass());
	}
}

void ABoundCollision::DestroySelf()
{
	Destroy();
}

void ABoundCollision::OnBeginOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
			

			playerCharacter->LaunchCharacter(direction * launchForce, true, true);
			ApplyDamageToTarget(OtherActor, 20);

		}
	}
}

