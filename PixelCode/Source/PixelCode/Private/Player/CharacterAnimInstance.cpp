// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CharacterAnimInstance.h"
#include "Player/PlayerOrganism.h"



void UCharacterAnimInstance::UpdateCombatType_Implementation(EWeaponType eType)
{
	weaponType = eType;
}

void UCharacterAnimInstance::UpdateCombatEnabled_Implementation(bool bEnable)
{
	bCombatEnable = bEnable;
}

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	myCharacter = Cast<APlayerOrganism>(GetOwningActor());


	if (myCharacter != nullptr)
	{
		playerMovementComp = myCharacter->GetCharacterMovement();
	}
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{

	if (myCharacter == nullptr)
		return;

	if (myCharacter != nullptr)
	{
		bDie = myCharacter->bDead;
	}
}


void UCharacterAnimInstance::AnimNotify_RDash()
{
	if (myCharacter != nullptr)
	{
		myCharacter->SkillR = true;
	}
}

void UCharacterAnimInstance::AnimNotify_blockcounter()
{
	if (myCharacter != nullptr)
	{
		myCharacter->CharcurrentLoc();
		myCharacter->MoveToTargetLocation();
		myCharacter->SkillE = true;
	}

}

