// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DemonSword.generated.h"

UCLASS()
class PIXELCODE_API ADemonSword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADemonSword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UStaticMeshComponent* swordComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UBoxComponent* damageBox;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UNiagaraSystem* swordHitNA;


	float currentTime;

	float moveDuration = 2.0f;

	float moveSpeed = 200.0f;

	FVector upLoc;

	FVector direction;

	FVector upNewLocation;

	FRotator playerRot;

	FVector targetLocation;

	FVector startLocation;

	FVector setDirection;

	FVector setplayerLoc;

	FQuat newQuat;

	float swordMaxHP = 30.0f;

	float swordCurrentHP;

	void SwordTakeDamage(float Damage);

	UPROPERTY(EditAnywhere, Category="MyMaterial")
	class UMaterial* damageMaterial;

	FTimerHandle timerhandle_SetOriginMatetrial;

	void SetOriginMaterial();

	UPROPERTY(EditAnywhere, Category="MyMaterial")
	class UMaterial* originalMaterial;

	bool directionSet = false;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class APixelCodeCharacter* player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	TSubclassOf<UCameraShakeBase> cameraShakeOBJ;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UNiagaraSystem* groundSwordImpact;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UParticleSystem* grounSwordImpact2;

	float startTime;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* UpSwordSound1;

	bool onceSound = false;

	bool onceSound2 = false;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* UpSwordSound2;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* UpSwordSound3;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* swingSwordSound1;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* swingSwordSound2;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* swingSwordSound3;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ApplyDamageToTarget(AActor* OtherActor, float DamageAmount);

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class APlayerController* Pc;

	

	UFUNCTION()
	void OnBeginOverlapSwordFloor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
