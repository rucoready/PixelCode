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
		myCharacter->CharcurrentLoc();
	}
}

void UCharacterAnimInstance::AnimNotify_blockcounter()
{
	if (myCharacter != nullptr)
	{
		myCharacter->SkillE = true;
		myCharacter->CharcurrentLoc();
		//myCharacter->MoveToTargetLocation();
	}

}

void UCharacterAnimInstance::AnimNotify_bRotation()
{
	if (myCharacter != nullptr)
	{
			myCharacter->bRotation = true;
			UE_LOG(LogTemp,Warning,TEXT("bRotation"));
			myCharacter->bUseSkill = true;
	}
}

void UCharacterAnimInstance::AnimNotify_bEndRotation()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bRotation = false;
		UE_LOG(LogTemp, Warning, TEXT("!bRotation"));
		myCharacter->bUseSkill = false;
	}
}



