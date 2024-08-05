// ADestroyAllActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestroyAllActor.generated.h"

UCLASS()
class PIXELCODE_API ADestroyAllActor : public AActor
{
	GENERATED_BODY()

public:
	ADestroyAllActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UBoxComponent* damageBoxComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class APlayerOrganism* Player;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class APlayerController* Pc;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UNiagaraSystem* swordDestroyNiagara;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* damageSound1;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* damageSound2;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* damageSound3;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* damageSound4;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* damageSound5;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* damageSound6;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* damageSound7;

	bool activeCollisionOnce = false;

	float currentTime;

	FTimerHandle timerHandle_ApplyDamage;

	bool bAppliedInitialDamage = false;

	bool bCollisionEnabled = false;

	void StartPeriodicDamageApplication(AActor* OtherActor, float damageAmount);

	bool overlapOnce[40];

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ApplyDamageToTarget(AActor* OtherActor, float DamageAmount);

	void PlayDamageSound();

	UFUNCTION()
	void OnBeginOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
