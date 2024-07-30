// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SpawnSkillActor/PlayerMageRightAttackSpawnActor.h"
#include "Boss/BossApernia.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>
#include "DemonSword.h"
#include "Grux.h"
#include "DogBart.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SphereComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Particles/ParticleSystem.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Particles/ParticleSystemComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/OverlapResult.h>

// Sets default values
APlayerMageRightAttackSpawnActor::APlayerMageRightAttackSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("boxComp"));
	SphereComp->SetupAttachment(RootComponent);

	SphereComp->SetGenerateOverlapEvents(true);

	NA_MageRightAttackComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	NA_MageRightAttackComp->SetupAttachment(SphereComp);

}

// Called when the game starts or when spawned
void APlayerMageRightAttackSpawnActor::BeginPlay()
{
	Super::BeginPlay();

	FVector forVec = GetActorUpVector() * 200.0f;
	FVector rightVec = GetActorRightVector() * 200.0f;
	FVector leftVec = -(GetActorRightVector() * 200.0f);

	

	int32 randomIndex = FMath::RandRange(0, 2); // 0, 1, or 2

	// Choose the vector based on the random index
	FVector ranVec;
	switch (randomIndex)
	{
	case 0:
		ranVec = forVec;
		break;
	case 1:
		ranVec = rightVec;
		break;
	case 2:
		ranVec = leftVec;
		break;
	}

	SetActorLocation(ranVec);


	NA_MageRightAttackComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageRightAttack, GetActorLocation(), GetActorRotation());


	
	

	// Set the actor's rotation
	

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerMageRightAttackSpawnActor::OnOverlapEnemy);
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_MageRightAttack, GetActorLocation(), GetActorRotation());


	for (TActorIterator<ABossApernia> var(GetWorld()); var; ++var)
	{
		Enemys = *var;
	}
	
	for (TActorIterator<AGrux> var(GetWorld()); var; ++var)
	{
		Enemys = *var;
	}

	for (TActorIterator<ADogBart> var(GetWorld()); var; ++var)
	{
		Enemys = *var;
	}




}

// Called every frame
void APlayerMageRightAttackSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime; // Speed는 앞으로 이동할 속도
	SetActorLocation(NewLocation);

	if (bDestroy)
	{
		DestroyTime += DeltaTime;
		if (DestroyTime >= 1.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("destroy"));
			bDestroy = false;
			DestroyTime = 0.0f;
			Destroy();
		}
	}

}

void APlayerMageRightAttackSpawnActor::OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABossApernia* boss = Cast<ABossApernia>(OtherActor);
	ADemonSword* demonSword = Cast<ADemonSword>(OtherActor);
	AGrux* grux = Cast<AGrux>(OtherActor);
	ADogBart* dogBart = Cast<ADogBart>(OtherActor);
	if (boss)
	{
		// 데미지 적용 예시: TakeDamage 함수 호출
		boss->BossTakeDamage(DamageAmount);
	}
	else if (demonSword)
	{
		demonSword->SwordTakeDamage(DamageAmount);
	}
	else if (grux)
	{
		grux->GruxTakeDamage(DamageAmount);
	}
	else if (dogBart)
	{
		dogBart->DogBartTakeDamage(DamageAmount);
	}
}

void APlayerMageRightAttackSpawnActor::CheckForObjectsInRadius()
{
	

}

