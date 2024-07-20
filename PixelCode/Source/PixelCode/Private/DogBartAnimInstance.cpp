// Fill out your copyright notice in the Description page of Project Settings.


#include "DogBartAnimInstance.h"
#include "DogBart.h"

void UDogBartAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	dogBart = Cast<ADogBart>(TryGetPawnOwner());
}

void UDogBartAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (dogBart == nullptr)
		return;

	auto vel = dogBart->GetVelocity();

	Vertical = FVector::DotProduct(vel, dogBart->GetActorForwardVector());


	Horizontal = FVector::DotProduct(vel, dogBart->GetActorRightVector());

	PitchAngle = dogBart->GetBaseAimRotation().GetNormalized().Pitch;

	PitchAngle = FMath::Clamp(-PitchAngle, -60, 60);

	
}

void UDogBartAnimInstance::AnimNotify_CollisionOn()
{
	dogBart->DamageCollisionActive();

}

void UDogBartAnimInstance::AnimNotify_CollisionOff()
{
	dogBart->DamageCollisionDeactive();
}
