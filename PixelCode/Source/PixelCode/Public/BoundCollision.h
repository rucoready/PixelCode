// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoundCollision.generated.h"

UCLASS()
class PIXELCODE_API ABoundCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoundCollision();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* sceneComponent;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USphereComponent* sphereComp;

	void ApplyDamageToTarget(AActor* OtherActor, float DamageAmount);

	FTimerHandle timerhandle_DestroySelf;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float launchForce = 1500.0f;

	void DestroySelf();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class APlayerController* Pc;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class APixelCodeCharacter* Player;

	UFUNCTION()
	void OnBeginOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
