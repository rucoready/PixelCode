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
#include "Kismet/GameplayStatics.h"
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
	targetLocation = startLocation + FVector(0, 0, -400);
}

// Called when the game starts or when spawned
void ADemonSword::BeginPlay()
{
	Super::BeginPlay();
	
	upLoc = GetActorLocation() + FVector(0, 0, 500);

	// 현재 위치와 upLoc 사이의 방향 벡터를 계산합니다.
	direction = (upLoc - GetActorLocation()).GetSafeNormal();

	// 이동 속도를 계산합니다.
	upNewLocation = GetActorLocation() + FVector(0, 0, 500);

	
	damageBox->OnComponentBeginOverlap.AddDynamic(this, &ADemonSword::OnBeginOverlapSwordFloor);
	
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


	if (currentTime > 5.0f && currentTime <= 8.0f)
	{		

		// InterEaseOut를 사용하여 새로운 위치로 서서히 이동합니다.
		FVector upNewLocation2 = FMath::InterpEaseOut(GetActorLocation(), upNewLocation, (currentTime - 5)/3 ,1.2f);

		// 새로운 위치로 이동합니다.
		SetActorLocation(upNewLocation2);
		
	
	}
	if (currentTime > 4.0f &&!directionSet)
	{
		TArray<AActor*> foundCharacters;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

		int32 randomIndex = FMath::RandRange(0, foundCharacters.Num() - 1);
		player = Cast<APixelCodeCharacter>(foundCharacters[randomIndex]);

		directionSet = true;

		setplayerLoc = player->GetActorLocation();
		setDirection = player->GetActorLocation() - GetActorLocation();

	}
	if (currentTime > 5.5f && currentTime <= 7.0f)
	{
		
		FVector directionToPlayer = player->GetActorLocation() - GetActorLocation();
	

		FRotator baseRotation = FRotationMatrix::MakeFromZ(-directionToPlayer).Rotator();

		FQuat currentQuat = GetActorRotation().Quaternion();
		FQuat targetQuat = baseRotation.Quaternion();

		float lerpAlpha = FMath::Clamp((currentTime - 5.5f) / 2.5f, 0.0f, 1.0f);

		newQuat = FQuat::Slerp(currentQuat, targetQuat, lerpAlpha);
		SetActorRotation(newQuat);
	}
	if (currentTime > 8.0f && currentTime < 15.f)
	{
		// 현재 회전 방향을 가져옴
		FVector downDirection = -newQuat.GetUpVector();

		// 속도 설정 (200 units per second)
		float speed = 7000.0f;

		// DeltaTime을 사용하여 이동 거리 계산
		FVector movement = downDirection * speed * DeltaTime;

		// 현재 위치에 이동 거리 추가
		FVector stingLoc = GetActorLocation() + movement;

		// 새 위치로 이동
		SetActorLocation(stingLoc);
	}

	
	
}

void ADemonSword::OnBeginOverlapSwordFloor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor->GetName().Contains("Floor"))
	{
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("OVerlap Floor"));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), grounSwordImpact2, GetActorLocation(), GetActorRotation(), FVector(1.0f));
		//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundSwordImpact, GetActorLocation(), GetActorRotation(), FVector(3.0f));
	}

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
				UE_LOG(LogTemp, Warning, TEXT("Trying to shake camera!"));
				pc->ClientStartCameraShake(cameraShakeOBJ);


			}
		}
	}
	
	
}



