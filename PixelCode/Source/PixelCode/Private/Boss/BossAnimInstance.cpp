// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossAnimInstance.h"
#include "Boss/TestBoss.h"
#include "BossSword.h"
#include "Boss/BossApernia.h"



void UBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	boss = Cast<ABossApernia>(TryGetPawnOwner());
	
	
	
}

void UBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (boss == nullptr)
		return;

	auto vel = boss->GetVelocity();
	
	Vertical = FVector::DotProduct(vel, boss->GetActorForwardVector());

	
	Horizontal = FVector::DotProduct(vel, boss->GetActorRightVector());

	PitchAngle = boss->GetBaseAimRotation().GetNormalized().Pitch;
	
	PitchAngle = FMath::Clamp(-PitchAngle, -60, 60);

	//UE_LOG(LogTemp, Warning, TEXT("Vertical: %f, Horizontal: %f, PitchAngle: %f"), Vertical, Horizontal, PitchAngle);

	
}

void UBossAnimInstance::AnimNotify_SwordOn()
{
	
	APawn* owningPawn = TryGetPawnOwner();
	if (owningPawn)
	{
		ABossApernia* bossCharacter = Cast<ABossApernia>(owningPawn);
		if (bossCharacter)
		{
			bossCharacter->bossBackSwordComp->SetVisibility(false);
			bossCharacter->bossSwordComp->SetVisibility(true);
		}
	}
}

void UBossAnimInstance::AnimNotify_CollisionOn()
{

	currentTime = 0;
		
	boss->SwordCollisionActive();

	attackCoolTime = true;

		
	
	

	
	
}

void UBossAnimInstance::AnimNotify_CollisionOff()
{
	
	boss->SwordCollisionDeactive();
	attackCoolTime = false;
}

void UBossAnimInstance::AnimNotify_Counter1On()
{
	boss->SetCounterMaterial();
	canCounterAttack = true;
}

void UBossAnimInstance::AnimNotify_Counter1Off()
{
	boss->SetOriginMaterial();
	canCounterAttack = false;
}





void UBossAnimInstance::AnimNotify_StingAttack()
{
	
}


