// Fill out your copyright notice in the Description page of Project Settings.


#include "BossSword.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h>
#include "Player/PlayerOrganism.h"

// Sets default values
ABossSword::ABossSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	swordComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("swordComp"));
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	damageSphereComp = CreateDefaultSubobject<UBoxComponent>(TEXT("damageSphereComp"));

	

	SetRootComponent(sceneComp);
	swordComp->SetupAttachment(RootComponent);
	damageSphereComp->SetupAttachment(RootComponent);
	damageSphereComp->SetRelativeLocation(FVector(0, 340, 10));
	damageSphereComp->SetRelativeRotation(FRotator(0, 0, 90));
	damageSphereComp->SetWorldScale3D(FVector(1.12, 0.46, 5.85));
	swordComp->SetWorldScale3D(FVector(0.7, 0.6, 1.0));

	bossCollisionComponent = CreateDefaultSubobject<UBossCollisionComponent>(TEXT("CollisionComponent"));


}


void ABossSword::BeginPlay()
{
	Super::BeginPlay();

	damageSphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABossSword::OnBeginOverlapSwordCollision);
	damageSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//damageSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Player = Cast<APlayerOrganism>(GetOwner());
	

}

// Called every frame
void ABossSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString CollisionString = UKismetStringLibrary::Conv_IntToString(damageSphereComp->GetCollisionEnabled());

	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Emerald, CollisionString);

}

void ABossSword::ApplyDamageToTarget(AActor* OtherActor, float DamageAmount)
{
	if (IsValid(OtherActor))
	{
		Pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, Pc, Player, UDamageType::StaticClass());
	}
}


void ABossSword::Testing1()
{
}

void ABossSword::RestoreOverlap()
{
	coolTimeOverlap = false;
}

void ABossSword::OnBeginOverlapSwordCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains("Player")&&!coolTimeOverlap)
	{
		GetWorldTimerManager().SetTimer(timerhandle_CoolTimeOverlap, this, &ABossSword::RestoreOverlap, 1.0f, false);
		coolTimeOverlap = true;
		Player = Cast<APlayerOrganism>(OtherActor);
		if (Player)
		{
			Player->GetHit(SweepResult.ImpactPoint,false);
		}
		ApplyDamageToTarget(OtherActor, 20);
	}
	
}

void ABossSword::SwordCollisionActive()
{

	damageSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
}

void ABossSword::SwordCollisionDeactive()
{
	damageSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}



