// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "TestBoss.generated.h"

UCLASS()
class PIXELCODE_API ATestBoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere,Category="MySetting")
	class UBehaviorTree* tree;

	UBehaviorTree* GetBehaviorTree() const;

	class UAIPerceptionStimuliSourceComponent* StimulusSource;
	void SetupStimulusSource();

};
