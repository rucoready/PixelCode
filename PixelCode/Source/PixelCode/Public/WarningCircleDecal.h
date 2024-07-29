// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "WarningCircleDecal.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API AWarningCircleDecal : public ADecalActor
{
	GENERATED_BODY()
	

public:
	// 생성자
	AWarningCircleDecal();


	// 데칼 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UDecalComponent* decalComponentOut;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UDecalComponent* decalComponentIn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* sceneComponent;

	virtual void Tick(float DeltaTime) override;

	float currentTime;

	float scaleSpeed;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class ADecalSpawnSword> decalSword;

	bool onceSpawnSword = false;

	float moveUpDuration = 0.5f; // 위로 이동할 시간
	float moveDownDuration = 2.0f; // 아래로 이동할 시간
	float moveUpDistance = 50.0f; // 위로 이동할 거리
	float moveDownDistance = -200.0f; // 아래로 이동할 거리

	float moveStartTime = 3.0f; // 이동이 시작되는 시간
	float moveEndTime = moveStartTime + moveUpDuration + moveDownDuration; // 이동이 끝나는 시간

	bool onceExplosion = false;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UNiagaraSystem* explosionSword;
};
