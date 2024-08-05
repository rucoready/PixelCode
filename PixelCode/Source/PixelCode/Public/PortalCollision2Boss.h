// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalCollision2Boss.generated.h"

UCLASS()
class PIXELCODE_API APortalCollision2Boss : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortalCollision2Boss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UBoxComponent* boxCollision;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* portalCreateSound;

	UFUNCTION()
	void OnBeginOverlapPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable)
	void ServerRPC_LevelMoveBoss();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_LevelMoveBoss();

	void RemoveBossLoadingUI();

	FTimerHandle timehandle_RemoveBossLoadingUI;
};
