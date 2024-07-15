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

	bool directionSet = false;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class APixelCodeCharacter* player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	TSubclassOf<UCameraShakeBase> cameraShakeOBJ;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UNiagaraSystem* groundSwordImpact;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UParticleSystem* grounSwordImpact2;

	UFUNCTION()
	void OnBeginOverlapSwordFloor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
