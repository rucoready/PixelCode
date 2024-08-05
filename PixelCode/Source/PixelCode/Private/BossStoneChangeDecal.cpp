// Fill out your copyright notice in the Description page of Project Settings.


#include "BossStoneChangeDecal.h"
#include "Components/DecalComponent.h"
#include "Math/UnrealMathUtility.h"
#include "DecalSpawnSword.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Player/PixelCodeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"

ABossStoneChangeDecal::ABossStoneChangeDecal()
{
	decalStoneSurface = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponentOut"));
	decalStoneSurface->SetupAttachment(RootComponent);
	decalStoneSurface->SetRelativeLocation(FVector(38.064628,0,0));
	decalStoneSurface->SetRelativeRotation(FRotator(0,180,180));
	decalStoneSurface->SetWorldScale3D(FVector(0.25,1,1));
	PrimaryActorTick.bCanEverTick = true;  
}

void ABossStoneChangeDecal::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABossStoneChangeDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentTime += DeltaTime;
	
	if (currentTime >= 0.0f && currentTime <= 2.0f)
	{
		FVector currentScale = decalStoneSurface->GetRelativeScale3D();  // 로컬 스케일 가져오기

		// X축만 증가
		float previousXScale = currentScale.X;
		currentScale.X += 0.005f;
		decalStoneSurface->SetRelativeScale3D(currentScale);  // 로컬 스케일 설정

		// X축 스케일 증가로 인한 위치 조정
		FVector currentLocation = decalStoneSurface->GetRelativeLocation();
		float deltaScaleX = currentScale.X - previousXScale;  // 스케일 증가량
		currentLocation.X += deltaScaleX * 0.5f;  // 위치를 스케일 증가량에 비례하여 이동
		decalStoneSurface->SetRelativeLocation(currentLocation);  // 위치 설정
	}


}
