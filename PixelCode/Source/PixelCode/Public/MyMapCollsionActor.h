// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyMapCollsionActor.generated.h"

UCLASS()
class PIXELCODE_API AMyMapCollsionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyMapCollsionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UBoxComponent* boxMapCollision;

	UFUNCTION()
	void OnBeginOverlapMapPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable)
	void ServerRPC_LevelMoveMap();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_LevelMoveMap();

	void RemoveMapLoadingUI();

	FTimerHandle timehandle_RemoveBossLoadingUI;

};
