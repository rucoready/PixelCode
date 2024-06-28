// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/TestBoss.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values
ATestBoss::ATestBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetupStimulusSource();

}

// Called when the game starts or when spawned
void ATestBoss::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* ATestBoss::GetBehaviorTree() const
{
	return tree;
}

void ATestBoss::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource)
	{
		
		StimulusSource->RegisterForSense(TSubclassOf <UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

