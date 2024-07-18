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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings")
	FVector moveDirection;

	UPROPERTY()
	APlayerOrganism* myCharacter;

	virtual void UpdateCombatType_Implementation(EWeaponType eType) override;
	virtual void UpdateCombatEnabled_Implementation(bool bEnable) override;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY()
	class UCharacterMovementComponent* playerMovementComp;


	//플레이어 E
	UFUNCTION()
	void AnimNotify_blockcounter(); 

	//플레이어 공격 중 회전 제어
	UFUNCTION()
	void AnimNotify_bRotation();

	UFUNCTION()
	void AnimNotify_bEndRotation();

	UFUNCTION()
	void AnimNotify_NSQ();

	UFUNCTION()
	void AnimNotify_NSR();

	UFUNCTION()
	void AnimNotify_BossGroggy();

	UFUNCTION()
	void AnimNotify_notBossGroggy();

	// 플레이어 z 스킬 대쉬
	UFUNCTION()
	void AnimNotify_PlayerZSkillDash();

};


