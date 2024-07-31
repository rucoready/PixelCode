// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SpawnSkillActor/PlayerMageZSkillSpawnActor.h"
#include "Boss/BossApernia.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>
#include "DemonSword.h"
#include "Grux.h"
#include "DogBart.h"
#include "BoundCollision.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SphereComponent.h>
#include "PCodePlayerController.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Particles/ParticleSystem.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Player/PixelCodeCharacter.h"


// Sets default values
APlayerMageZSkillSpawnActor::APlayerMageZSkillSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(RootComponent);

	SphereComp->SetGenerateOverlapEvents(true);

	SphereComp->SetRelativeScale3D(FVector(40));
	
}

// Called when the game starts or when spawned
void APlayerMageZSkillSpawnActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorLocation(GetActorLocation());
	bDestroy = true;
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerMageZSkillSpawnActor::OnOverlapEnemy);
	bMagicCircle = true;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MagicCircle, GetActorLocation(), GetActorRotation());
}

// Called every frame
void APlayerMageZSkillSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMagicCircle)
	{
		MagicCircleAttackSpawnTime += DeltaTime;
		if (MagicCircleAttackSpawnTime >= 1.0f)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageZSkillAttack, GetActorLocation(), GetActorRotation());
			MagicCircleAttackSpawnTime = 0.0f;
			bMagicCircle = false;
		}
	}


	if (bDestroy)
	{
		DestroyTime += DeltaTime;
		if (DestroyTime >= 5.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("destroy"));
			bDestroy = false;
			DestroyTime = 0.0f;
			GetWorldTimerManager().ClearTimer(DamageTimerHandle);
			Destroy();
		}
	}
}


void APlayerMageZSkillSpawnActor::ApplyDamage()
{
	if (OverlappingActor == boss && bIsOverlapping)
	{
		
		// 데미지 적용 예시: TakeDamage 함수 호출
		boss->BossTakeDamage(DamageAmount);
		//boss->ServerRPC_TakeDamage();
		if (NA_MageZSkillhit != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageZSkillhit, OverlappingActor->GetActorLocation(), OverlappingActor->GetActorRotation());
		}
		
	}
	if (OverlappingActor == demonSword && bIsOverlapping)
	{
		demonSword->SwordTakeDamage(DamageAmount);
		if (NA_MageZSkillhit != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageZSkillhit, OverlappingActor->GetActorLocation(), OverlappingActor->GetActorRotation());
		}
	}
	if (OverlappingActor == grux && bIsOverlapping)
	{
		grux->GruxTakeDamage(DamageAmount);
		grux->ServerRPC_TakeDamage();
		if (NA_MageZSkillhit != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageZSkillhit, OverlappingActor->GetActorLocation(), OverlappingActor->GetActorRotation());
			UE_LOG(LogTemp, Warning, TEXT("Gruxdamage"));
		}
	}
	if (OverlappingActor == dogBart && bIsOverlapping)
	{
		dogBart->DogBartTakeDamage(DamageAmount);
		dogBart->ServerRPC_TakeDamage();


		if (NA_MageZSkillhit != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageZSkillhit, OverlappingActor->GetActorLocation(), OverlappingActor->GetActorRotation());
		}
		UE_LOG(LogTemp, Warning, TEXT("dogbartdamage"));
	}
}





void APlayerMageZSkillSpawnActor::OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	boss = Cast<ABossApernia>(OtherActor);
	demonSword = Cast<ADemonSword>(OtherActor);
	grux = Cast<AGrux>(OtherActor);
	dogBart = Cast<ADogBart>(OtherActor);

	if (OtherActor && OtherActor != this && OtherActor == boss)
	{
		OverlappingActor = OtherActor;
		bIsOverlapping = true;

		GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &APlayerMageZSkillSpawnActor::ApplyDamage, 0.5f, true);
	}

	if (OtherActor && OtherActor != this && OtherActor == demonSword)
	{
		OverlappingActor = OtherActor;
		bIsOverlapping = true;

		GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &APlayerMageZSkillSpawnActor::ApplyDamage, 0.5f, true);
	}

	if (OtherActor && OtherActor != this && OtherActor == grux)
	{
		OverlappingActor = OtherActor;
		bIsOverlapping = true;

		GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &APlayerMageZSkillSpawnActor::ApplyDamage, 0.5f, true);
	}

	if (OtherActor && OtherActor != this && OtherActor == dogBart)
	{
		OverlappingActor = OtherActor;
		bIsOverlapping = true;

		GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &APlayerMageZSkillSpawnActor::ApplyDamage, 0.5f, true);
	}




	//if (boss)
	//{
	//	// 데미지 적용 예시: TakeDamage 함수 호출
	//	boss->BossTakeDamage(DamageAmount);
	//	if (NA_MageZSkillhit != nullptr)
	//	{
	//		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageZSkillhit, OtherActor->GetActorLocation(), OtherActor->GetActorRotation());
	//	}
	//}

	//else if (demonSword)
	//{
	//	demonSword->SwordTakeDamage(DamageAmount);
	//	if (NA_MageZSkillhit != nullptr)
	//	{
	//		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageZSkillhit, OtherActor->GetActorLocation(), OtherActor->GetActorRotation());
	//	}
	//}
	//else if (grux)
	//{
	//	grux->GruxTakeDamage(DamageAmount);
	//	grux->ServerRPC_TakeDamage();
	//	if (NA_MageZSkillhit != nullptr)
	//	{
	//		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageZSkillhit, OtherActor->GetActorLocation(), OtherActor->GetActorRotation());
	//		UE_LOG(LogTemp, Warning, TEXT("Gruxdamage"));
	//	}
	//}
	//else if (dogBart)
	//{
	//	dogBart->DogBartTakeDamage(DamageAmount);
	//	dogBart->ServerRPC_TakeDamage();
	//	

	//	if (NA_MageZSkillhit != nullptr)
	//	{
	//		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageZSkillhit, OtherActor->GetActorLocation(), OtherActor->GetActorRotation());
	//	}
	//	UE_LOG(LogTemp, Warning, TEXT("dogbartdamage"));
	//}



}

void APlayerMageZSkillSpawnActor::OnEndOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == OverlappingActor)
	{
		bIsOverlapping = false;
		OverlappingActor = nullptr;

		GetWorldTimerManager().ClearTimer(DamageTimerHandle);
	}
}

