// Fill out your copyright notice in the Description page of Project Settings.


#include "DemonSword.h"
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

// Sets default values
ADemonSword::ADemonSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	swordComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("swordComp"));
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	damageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("damageBox"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> bossOBJ(TEXT("/Script/Engine.StaticMesh'/Game/KMS_AI/Boss_Alpernia/DemonSword/DemonSword.DemonSword'"));
	if (bossOBJ.Succeeded())
	{
		swordComp->SetStaticMesh(bossOBJ.Object);
	}


	SetRootComponent(sceneComp);
	swordComp->SetupAttachment(RootComponent);
	damageBox->SetupAttachment(RootComponent);

	swordComp->SetRelativeLocation(FVector(0, 0, 560));
	swordComp->SetRelativeRotation(FRotator(0, 180, 180));
	swordComp->SetWorldScale3D(FVector(3.0));


	damageBox->SetRelativeLocation(FVector(0, 0, 307));
	damageBox->SetRelativeRotation(FRotator(0, 0, 0));
	damageBox->SetWorldScale3D(FVector(1.75, 0.25, 7.0));
	swordComp->SetWorldScale3D(FVector(2.0));

	startLocation = swordComp->GetRelativeLocation();
	targetLocation = startLocation + FVector(0, 0, -200);
}

// Called when the game starts or when spawned
void ADemonSword::BeginPlay()
{
	Super::BeginPlay();
	
	upLoc = GetActorLocation() + FVector(0, 0, 800);

	// 현재 위치와 upLoc 사이의 방향 벡터를 계산합니다.
	direction = (upLoc - GetActorLocation()).GetSafeNormal();

	// 이동 속도를 계산합니다.
	upNewLocation = GetActorLocation() + FVector(0, 0, 800);

	
	damageBox->OnComponentBeginOverlap.AddDynamic(this, &ADemonSword::OnBeginOverlapSwordFloor);
	
	
	
	swordCurrentHP = swordMaxHP;
}

// Called every frame
void ADemonSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentTime += DeltaTime;

	//Set Speed *2
	float adjustDuration = moveDuration / 3.2f;

	float alpha = FMath::Clamp(currentTime / adjustDuration, 0.0f, 1.0f);
	float easedAlpha = 1.0f - FMath::Pow(1.0f - alpha, 2.0f); // Quadratic easing out

	FVector newLocation = FMath::Lerp(startLocation, targetLocation, easedAlpha);

	swordComp->SetRelativeLocation(newLocation);


	if (currentTime > 15.0f && currentTime <= 18.0f)
	{		

		// InterEaseOut를 사용하여 새로운 위치로 서서히 이동합니다.
		FVector upNewLocation2 = FMath::InterpEaseOut(GetActorLocation(), upNewLocation, (currentTime - 15)/3 ,1.2f);

		// 새로운 위치로 이동합니다.
		SetActorLocation(upNewLocation2);

		if (!onceSound)
		{
			onceSound = true;
			int32 value = FMath::RandRange(1, 3);
			{
				if (value == 1)
				{
					UGameplayStatics::PlaySoundAtLocation(this, UpSwordSound1, GetActorLocation());
				}
				else if (value == 2)
				{
					UGameplayStatics::PlaySoundAtLocation(this, UpSwordSound2, GetActorLocation());
				}
				else
				{
					UGameplayStatics::PlaySoundAtLocation(this, UpSwordSound3, GetActorLocation());
				}
			}
		}
		
		
	
	}
	if (currentTime > 14.0f &&!directionSet)
	{
		TArray<AActor*> foundCharacters;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

		int32 randomIndex = FMath::RandRange(0, foundCharacters.Num() - 1);
		player = Cast<APixelCodeCharacter>(foundCharacters[randomIndex]);

		directionSet = true;

		setplayerLoc = player->GetActorLocation();
		setDirection = player->GetActorLocation() - GetActorLocation();

	}
	if (currentTime > 15.5f && currentTime <= 17.0f)
	{
		
		FVector directionToPlayer = player->GetActorLocation() - GetActorLocation();
	

		FRotator baseRotation = FRotationMatrix::MakeFromZ(-directionToPlayer).Rotator();

		FQuat currentQuat = GetActorRotation().Quaternion();
		FQuat targetQuat = baseRotation.Quaternion();

		float lerpAlpha = FMath::Clamp((currentTime - 15.5f) / 2.5f, 0.0f, 1.0f);

		newQuat = FQuat::Slerp(currentQuat, targetQuat, lerpAlpha);
		SetActorRotation(newQuat);
	}
	if (currentTime > 18.0f && currentTime <23.f)
	{
		// 현재 회전 방향을 가져옴
		FVector downDirection = -newQuat.GetUpVector();

		// 속도 설정 (200 units per second)
		float speed = 9000.0f;

		// DeltaTime을 사용하여 이동 거리 계산
		FVector movement = downDirection * speed * DeltaTime;

		// 현재 위치에 이동 거리 추가
		FVector stingLoc = GetActorLocation() + movement;

		// 새 위치로 이동
		SetActorLocation(stingLoc);

		



	}

	if (swordCurrentHP <= 0)
	{
		Destroy();
		onceSound = false;
		onceSound2 = false;

	}
	
}

void ADemonSword::SwordTakeDamage(float Damage)
{
	swordCurrentHP -= Damage;
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), swordHitNA, GetActorLocation(), GetActorRotation(), FVector(10.0f));
	// Material 변경
	if (damageMaterial)
	{
		if (swordComp)
		{
			int32 MaterialIndex = 0; 
			swordComp->SetMaterial(MaterialIndex, damageMaterial);

			GetWorldTimerManager().SetTimer(timerhandle_SetOriginMatetrial, this, &ADemonSword::SetOriginMaterial, 0.28f, false);
		}
	}
}

void ADemonSword::SetOriginMaterial()
{
	if (originalMaterial)
	{
		if (swordComp)
		{
			int32 MaterialIndex = 0; 
			swordComp->SetMaterial(MaterialIndex, originalMaterial);
		}
	}
}

void ADemonSword::ApplyDamageToTarget(AActor* OtherActor, float DamageAmount)
{
	if (IsValid(OtherActor))
	{
		Pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, Pc, player, UDamageType::StaticClass());


	}
}

void ADemonSword::OnBeginOverlapSwordFloor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (currentTime > 15.0f)
	{
		
	}
	if (OtherActor->GetName().Contains("Floor")|| OtherActor->GetName().Contains("Player")|| OtherActor->GetName().Contains("Cube"))
	{
		if (currentTime > 16.0f)
		{
			Destroy();
			onceSound = false;
			ApplyDamageToTarget(OtherActor, 7);
			
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), grounSwordImpact2, GetActorLocation(), GetActorRotation(), FVector(0.5f));
			//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundSwordImpact, GetActorLocation(), GetActorRotation(), FVector(3.0f));
			TArray<AActor*> foundCharacters;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

			for (AActor* actor : foundCharacters)
			{
				player = Cast<APixelCodeCharacter>(actor);
				if (player)
				{
					APlayerController* pc = player->GetController<APlayerController>();
					if (pc != nullptr)
					{

						pc->ClientStartCameraShake(cameraShakeOBJ);

						APixelCodeCharacter* playerCharacter = Cast<APixelCodeCharacter>(OtherActor);
						if (playerCharacter)
						{

							playerCharacter->GetHit(direction, true);
						
						}
					}
				}
			}

			if (!onceSound2)
			{
				onceSound2 = true;
				int32 value2 = FMath::RandRange(1, 3);
				{
					if (value2 == 1)
					{
						UGameplayStatics::PlaySoundAtLocation(this, swingSwordSound1, GetActorLocation());
					}
					else if (value2 == 2)
					{
						UGameplayStatics::PlaySoundAtLocation(this, swingSwordSound2, GetActorLocation());
					}
					else
					{
						UGameplayStatics::PlaySoundAtLocation(this, swingSwordSound3, GetActorLocation());
					}
				}
			}
		}
		

	}

	
	
	
}



