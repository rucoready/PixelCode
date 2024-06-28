// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY()
	class ABossApernia* boss;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MySettings")
	float Horizontal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MySettings")
	float Vertical;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MySettings")
	float PitchAngle;

	UFUNCTION(BlueprintCallable)
	void AnimNotify_StingAttack();


	UFUNCTION(BlueprintCallable)
	void AnimNotify_SwordOn();


	
	
};
