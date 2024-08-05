// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DragonRazorStatue.generated.h"

UCLASS()
class PIXELCODE_API ADragonRazorStatue : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADragonRazorStatue();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UStaticMeshComponent* statueComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class USphereComponent* sphereComp;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UNiagaraComponent* razorComponent;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UNiagaraComponent* fireComponent;

	void TakeDamageD(float damage);

	void OnMyClickBlackRoom();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* fractureSound;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* fractureHitSound;


	float currentHp;

	float maxHp = 30.0f;

	FTimerHandle timerhandle_Destroy;

	FTimerHandle timerhandle_SetOriginMaterial;

	bool onceSpawnFracture = false;

	bool onceDie = false;

	void SpawnFractureStatue();

	void DestroySelf();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class AFractureDragonStatue> fractureStatue;

	class AFractureDragonStatue* fractureMesh;

	AActor* fracturStatue;

	UFUNCTION()
	void DrestroyFractureStatue();

	UPROPERTY(EditAnywhere, Category="MyMaterial")
	class UMaterial* originalMaterial;

	UPROPERTY(EditAnywhere, Category="MyMaterial")
	class UMaterial* damagelMaterial;

	void RestoreOriginMaterial();

	int32 destroyCount;


	class ABossApernia* TargetPawn;

	UFUNCTION(Server, Reliable)
	void ServerRPC_DestroyCheck();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DestroyCheck();


};
