// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EXPActor.generated.h"

UCLASS()
class PIXELCODE_API AEXPActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEXPActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UStaticMeshComponent* expComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class USphereComponent* sphereComp;

	float RunningTime;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* expSound1;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* expSound2;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* expSound3;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* expSound4;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* expSound5;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* expSound6;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* expSound7;


	UFUNCTION()
	void OnBeginOverlapExpOrb(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	  // RPC function to add experience (to be called from the server)
    UFUNCTION(Client, Reliable)
    void ClientAddExp(APCodePlayerController* Pc, float ExpAmount);
};
