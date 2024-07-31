// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "BossStoneChangeDecal.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API ABossStoneChangeDecal : public ADecalActor
{
	GENERATED_BODY()
public:
	// 생성자
	ABossStoneChangeDecal();
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UDecalComponent* decalStoneSurface;

	float currentTime;

};
