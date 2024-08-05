// Fill out your copyright notice in the Description page of Project Settings.


#include "GruxAnimInstance.h"
#include "Grux.h"

void UGruxAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	grux = Cast<AGrux>(TryGetPawnOwner());
}

void UGruxAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (grux == nullptr)
		return;

	auto vel = grux->GetVelocity();

	Vertical = FVector::DotProduct(vel, grux->GetActorForwardVector());


	Horizontal = FVector::DotProduct(vel, grux->GetActorRightVector());

	PitchAngle = grux->GetBaseAimRotation().GetNormalized().Pitch;

	PitchAngle = FMath::Clamp(-PitchAngle, -60, 60);

	bAttack = grux->bPlayAttack;
	if (grux->bPlayAttack)
	{
		
	}
	if(grux->attackSensing == true &&!playonce)
	{
		playonce = true;
		
		
	}
}

void UGruxAnimInstance::AnimNotify_CollisionOn()
{
	grux->SwordCollisionActive();
}

void UGruxAnimInstance::AnimNotify_CollisionOff()
{
	grux->SwordCollisionDeactive();
}
