// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Player/AnimInstance_Interface.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */

class APlayerOrganism;
class APixelCodeCharacter;

UCLASS()
class PIXELCODE_API UCharacterAnimInstance : public UAnimInstance, public IAnimInstance_Interface
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType weaponType = EWeaponType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCombatEnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDie = false;

	//  구르기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRoll = false;

	UFUNCTION()
	void bRollState();



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings")
	FVector moveDirection;

	UPROPERTY()
	APlayerOrganism* myCharacter;

	UPROPERTY()
	APixelCodeCharacter* myPlayer;

	virtual void UpdateCombatType_Implementation(EWeaponType eType) override;
	virtual void UpdateCombatEnabled_Implementation(bool bEnable) override;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY()
	class UCharacterMovementComponent* playerMovementComp;


	UFUNCTION()
	void AnimNotify_RDash(); //플레이어 R

};


