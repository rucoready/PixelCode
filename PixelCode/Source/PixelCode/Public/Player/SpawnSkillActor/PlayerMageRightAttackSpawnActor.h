// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerMageRightAttackSpawnActor.generated.h"

class UParticleSystem;
class USceneComponent;
class USphereComponent;
class ACharacter;
class AActor;
class ABossApernia;
class ADemonSword;
class AGrux;
class ADogBart;



UCLASS()
class PIXELCODE_API APlayerMageRightAttackSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerMageRightAttackSpawnActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Components")  // 보통 VisibleAnywhere로 선언하여 에디터에서 보이도록 설정
    USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, Category = "Components");
	USphereComponent * SphereComp1;


	UPROPERTY(EditAnywhere, Category = "Components");
	USphereComponent* SphereComp2;

	UPROPERTY(EditAnywhere, Category = "Components");
	USphereComponent* SphereComp3;

	UPROPERTY(EditAnywhere, Category = "Components");
	USphereComponent* SphereComp4;

	FVector GetTargetEnemyLocation();

	UPROPERTY(EditAnywhere)
	UParticleSystem* NA_MageRightAttack;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* NA_MageRightAttackComp1;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* NA_MageRightAttackComp2;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* NA_MageRightAttackComp3;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* NA_MageRightAttackComp4;

	UPROPERTY()
	ACharacter* Enemys;

	float Speed = 500.f;

	float DestroyTime = 0.0f;

	bool bDestroy = false;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DamageAmount = 10;

	UFUNCTION()
	void OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void CheckForEnemiesAndAttack();

};
