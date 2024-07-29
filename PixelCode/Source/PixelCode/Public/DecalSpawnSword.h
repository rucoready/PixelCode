// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecalSpawnSword.generated.h"

UCLASS()
class PIXELCODE_API ADecalSpawnSword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADecalSpawnSword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UStaticMeshComponent* swordComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USceneComponent* sceneComp;

	float currentTime;

	

	// 이동할 Z축의 시작 및 종료 위치 정의
	float startZ = 0.0f;   // 초기 Z 위치
	float midZ = 250.0f;   // 1.2초 후의 Z 위치
	float endZ = -400.0f;  // 4.0초 후의 Z 위치

	// 시간에 따른 Z축 위치 계산
	float newZ = 0.0f;

};
