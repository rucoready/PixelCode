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
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
APlayerMageRightAttackSpawnActor::APlayerMageRightAttackSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;

	SphereComp1 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp1"));
	SphereComp1->SetupAttachment(RootComponent);

	SphereComp1->SetGenerateOverlapEvents(true);

	SphereComp2 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp2"));
	SphereComp2->SetupAttachment(RootComponent);

	SphereComp2->SetGenerateOverlapEvents(true);

	SphereComp3 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp3"));
	SphereComp3->SetupAttachment(RootComponent);

	SphereComp3->SetGenerateOverlapEvents(true);

	SphereComp4 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp4"));
	SphereComp4->SetupAttachment(RootComponent);

	SphereComp4->SetGenerateOverlapEvents(true);

	NA_MageRightAttackComp1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp1"));
	NA_MageRightAttackComp1->SetupAttachment(SphereComp1);

	NA_MageRightAttackComp2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp2"));
	NA_MageRightAttackComp2->SetupAttachment(SphereComp2);

	NA_MageRightAttackComp3 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp3"));
	NA_MageRightAttackComp3->SetupAttachment(SphereComp3);

	NA_MageRightAttackComp4 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp4"));
	NA_MageRightAttackComp4->SetupAttachment(SphereComp4);
}

// Called when the game starts or when spawned
void APlayerMageRightAttackSpawnActor::BeginPlay()
{
	Super::BeginPlay();


	CheckForEnemiesAndAttack();
	

	bDestroy = true;
	NA_MageRightAttackComp1 = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageRightAttack, GetActorLocation(), GetActorRotation());
	NA_MageRightAttackComp2 = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageRightAttack, GetActorLocation(), GetActorRotation());
	NA_MageRightAttackComp3 = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageRightAttack, GetActorLocation(), GetActorRotation());
	NA_MageRightAttackComp4 = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageRightAttack, GetActorLocation(), GetActorRotation());
	


	SphereComp1->OnComponentBeginOverlap.AddDynamic(this, &APlayerMageRightAttackSpawnActor::OnOverlapEnemy);
	SphereComp2->OnComponentBeginOverlap.AddDynamic(this, &APlayerMageRightAttackSpawnActor::OnOverlapEnemy);
	SphereComp3->OnComponentBeginOverlap.AddDynamic(this, &APlayerMageRightAttackSpawnActor::OnOverlapEnemy);
	SphereComp4->OnComponentBeginOverlap.AddDynamic(this, &APlayerMageRightAttackSpawnActor::OnOverlapEnemy);
	


	FVector PlayerLocation = GetActorLocation(); // �÷��̾� ��ġ

	// �߻� ��ġ�� ����մϴ�.
	//FVector LeftSpawnLocation = PlayerLocation - GetActorRightVector() * 300.f; // �÷��̾��� �������� 500 ������ŭ �̵�
	//FVector LeftTopSpawnLocation = PlayerLocation + GetActorUpVector() * 200.f - GetActorRightVector() * 300.f; // �÷��̾��� ���� ���� 200 ������ŭ �̵�
	//FVector RightSpawnLocation = PlayerLocation + GetActorRightVector() * 300.f; // �÷��̾��� ���������� 500 ������ŭ �̵�
	//FVector RightTopSpawnLocation = PlayerLocation + GetActorUpVector() * 200.f + GetActorRightVector() * 300.f; // �÷��̾��� ������ ���� 200 ������ŭ �̵�

	// �� ��ġ���� ���Ǿ��ø����� �߻��մϴ�.
	SphereComp1->SetWorldLocation(PlayerLocation);
	SphereComp2->SetWorldLocation(PlayerLocation);
	SphereComp3->SetWorldLocation(PlayerLocation);
	SphereComp4->SetWorldLocation(PlayerLocation);


	
}

// Called every frame
void APlayerMageRightAttackSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	//FVector NewLocation = GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime; // Speed�� ������ �̵��� �ӵ�
	
	// �÷��̾� �̵� ����
	FVector TargetLocation = GetTargetEnemyLocation();
	FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), TargetLocation, DeltaTime, Speed);
	SetActorLocation(NewLocation);


	if (bDestroy)
	{
		DestroyTime += DeltaTime;
		if (DestroyTime >= 2.5f)
		{
			UE_LOG(LogTemp, Warning, TEXT("destroy"));
			bDestroy = false;
			DestroyTime = 0.0f;
			Destroy();
		}
	}

	//CheckForEnemiesAndAttack();

}

FVector APlayerMageRightAttackSpawnActor::GetTargetEnemyLocation()
{
	// ���忡�� ���� ����� ���� ��ġ�� Ž���ϱ� ���� ����
	TArray<AActor*> FoundActors;
	FVector PlayerLocation = GetActorLocation(); // �÷��̾� ��ġ
	float AttackRange = 1000.f; // ���� ������ �ִ� �Ÿ�
	FVector TargetLocation = PlayerLocation; // �⺻������ �÷��̾� ��ġ���� ����

	// ���忡�� �÷��̾� �ֺ��� �� ã��
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		APawn* Pawn = Cast<APawn>(Actor);
		if (Pawn && !Pawn->IsPlayerControlled()) // �÷��̾� ĳ���ʹ� �����ϰ� ���� Ž��
		{
			float DistanceToEnemy = FVector::Dist(PlayerLocation, Pawn->GetActorLocation());
			if (DistanceToEnemy <= AttackRange)
			{
				// ���� ����� ���� ��ġ�� Ÿ������ ����
				TargetLocation = Pawn->GetActorLocation();
				break;
			}
		}
	}

	return TargetLocation;
}





void APlayerMageRightAttackSpawnActor::OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABossApernia* boss = Cast<ABossApernia>(OtherActor);
	ADemonSword* demonSword = Cast<ADemonSword>(OtherActor);
	AGrux* grux = Cast<AGrux>(OtherActor);
	ADogBart* dogBart = Cast<ADogBart>(OtherActor);
	if (boss)
	{
		// ������ ���� ����: TakeDamage �Լ� ȣ��
		boss->BossTakeDamage(DamageAmount);
		Destroy();
	}
	else if (demonSword)
	{
		demonSword->SwordTakeDamage(DamageAmount);
		Destroy();
	}
	else if (grux)
	{
		grux->GruxTakeDamage(DamageAmount);
		Destroy();
	}
	else if (dogBart)
	{
		dogBart->DogBartTakeDamage(DamageAmount);
		Destroy();
	}


	
}

void APlayerMageRightAttackSpawnActor::CheckForEnemiesAndAttack()
{
	// ���忡�� �� Ž���ϱ�
	TArray<AActor*> FoundActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)); // ���� Pawn �ݸ��� ä�ο� �־�� ��

	FVector PlayerLocation = GetActorLocation(); // �÷��̾� ��ġ
	float AttackRange = 1000.f; // ���� ������ �ִ� �Ÿ�

	// ���忡�� �÷��̾� �ֺ��� �� ã��
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		APawn* Pawn = Cast<APawn>(Actor);
		if (Pawn && !Pawn->IsPlayerControlled()) // �÷��̾� ĳ���ʹ� �����ϰ� ���� Ž��
		{
			float DistanceToEnemy = FVector::Dist(PlayerLocation, Pawn->GetActorLocation());
			if (DistanceToEnemy <= AttackRange)
			{
				// ���� ���� ���� ���� ���� ������ ���� ����
				SetActorLocation(Pawn->GetActorLocation());
			}
		}
	}
}







