// Fill out your copyright notice in the Description page of Project Settings.


#include "WarningCircleDecal.h"
#include "Components/DecalComponent.h"
#include "Math/UnrealMathUtility.h"
#include "DecalSpawnSword.h"
#include "Components/SceneComponent.h"
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
			Destroy();

		}
	}
	

	currentTime += DeltaTime;
}
