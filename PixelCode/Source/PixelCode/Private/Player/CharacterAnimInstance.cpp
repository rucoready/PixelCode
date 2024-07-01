// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CharacterAnimInstance.h"
#include "Player/PlayerOrganism.h"

#include "Player/PixelCodeCharacter.h"



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

	myPlayer = Cast<APixelCodeCharacter>(GetOwningActor());

	myCharacter = Cast<APlayerOrganism>(GetOwningActor());


	if (myCharacter != nullptr)
	{
		playerMovementComp = myCharacter->GetCharacterMovement();
	}
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (myPlayer == nullptr)
		return;

	if (myCharacter == nullptr)
		return;

	if (myCharacter != nullptr)
	{
		bDie = myCharacter->bDead;

		bRoll = myPlayer->bRoll;
	}
}

void UCharacterAnimInstance::bRollState()
{
	/*if (myPlayer)
	{
		myPlayer->bRoll = false;
	}*/
}

void UCharacterAnimInstance::AnimNotify_RDash()
{
	if (myCharacter != nullptr)
	{
		myCharacter->SkillR = true;
	}
	UE_LOG(LogTemp, Warning, TEXT("noti"))

}

