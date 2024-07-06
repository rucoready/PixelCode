// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BossSword.h"
#include "Components/ChildActorComponent.h"
#include "BossApernia.generated.h"

UCLASS()
class PIXELCODE_API ABossApernia : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossApernia();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "MySetting")
	class UBehaviorTree* tree;

	UBehaviorTree* GetBehaviorTree() const;

	class UAIPerceptionStimuliSourceComponent* StimulusSource;

	void SetupStimulusSource();

	//class UStaticMeshComponent* bossSwordComp;


	class UStaticMeshComponent* bossBackSwordComp;

	//UPROPERTY(EditAnywhere, Category="MySettings")
	//class ABossSword* bossSwordComp;

	//UPROPERTY(EditAnywhere, Category = "MySettings")
	//TSubclassOf<ABossSword> BossSwordActorClass;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UChildActorComponent* bossSwordComp;

	UFUNCTION()
	void SwordCollisionActive();

	UFUNCTION()
	void SwordCollisionDeactive();

	//UPROPERTY(EditAnywhere, Category = "MySettings")
	//class ABossSword* sword;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class ABossSword> bossSwordclass;

	float bossMaxHP = 200.0f;

	float bossCurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* bossTakeDamageMT;

	UFUNCTION()
	void BossTakeDamage(float Damage);

};
