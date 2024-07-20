// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DogBartAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UDogBartAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY()
	class ADogBart* dogBart;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MySettings")
	float Horizontal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MySettings")
	float Vertical;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MySettings")
	float PitchAngle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MySettings")
	bool bAttack = false;

	UFUNCTION(BlueprintCallable)
	void AnimNotify_CollisionOn();

	UFUNCTION()
	void AnimNotify_CollisionOff();
};
