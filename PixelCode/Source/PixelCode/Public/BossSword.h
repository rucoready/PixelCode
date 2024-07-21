// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossCollisionComponent.h"
#include "BossSword.generated.h"

UCLASS()
class PIXELCODE_API ABossSword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossSword();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")

	class UBossCollisionComponent* bossCollisionComponent;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class APlayerOrganism* Player;

	
	UPROPERTY(EditAnywhere, Category="MySettings")
	class APlayerController* Pc;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ApplyDamageToTarget(AActor* OtherActor, float DamageAmount);


	UPROPERTY(EditAnywhere, Category="MySettings")
	class UBoxComponent* damageSphereComp;

	void Testing1();

	FTimerHandle timerhandle_CoolTimeOverlap;

	void RestoreOverlap();

	bool coolTimeOverlap = false;

	UFUNCTION()
	void OnBeginOverlapSwordCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SwordCollisionActive();

	void SwordCollisionDeactive();
	
};
