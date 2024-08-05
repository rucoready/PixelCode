// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Grux.generated.h"

UCLASS()
class PIXELCODE_API AGrux : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGrux();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "MySetting")
	class UBehaviorTree* tree;

	UBehaviorTree* GetBehaviorTree() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UWidgetComponent* damageWidgetComponentl;

	class UDamageWidget* damageWidgetInstance;

	//Network

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* attack01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* attack02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* attack03;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* soulGetSound;

	bool bPlayAttack = false;

	bool attackSensing = false;

	FVector originLocation;

	FVector maxLocation;

	float maxLocationSize;

	FVector currentLocation;

	void CoolTimeSetting();

	UPROPERTY(EditAnywhere, Category="MySettings")
	float maxHp = 5000.0f;

	

	float currentHp;

	float distance;

	void GruxTakeDamage(float Damage);

	bool returnCoolTime = false;

	FTimerHandle timerhandle_ReturnOrigin;

	FTimerHandle timerhandle_RepocessBehaviorTree;

	FTimerHandle timerhandle_Destroy;

	bool dieOnce = false;

	bool gruxDie = false;

	void DestroySelf();

	void RepocessBehaviorTreeRe();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class APlayerOrganism* players;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* gruxTakeDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* gruxTakeDamage2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* gruxTakeDamage3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* gruxDie1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* gruxDie2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	TSubclassOf<UCameraShakeBase> cameraShakeOBJ;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UBoxComponent* boxCollision;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UBoxComponent* boxCollisionR;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class APixelCodeCharacter* Player;

	
	UPROPERTY(EditAnywhere, Category="MySettings")
	class APlayerController* Pc;

	void SwordCollisionActive();

	void SwordCollisionDeactive();

	bool cooltimeReset;

	int32 widgetRandomValue;

	FTimerHandle timerhandle_CoolTimeOverlap;

	int32 damageAmount;

	

	int32 gruxNewTakeDamage;

	void ResetOverlapCoolTime();

	UFUNCTION()
	void OnBeginOverlapSwordCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ApplyDamageToTarget(AActor* OtherActor, float DamageAmount);

 	UFUNCTION(Server, Reliable)
 	void ServerRPC_Attack01();
 
 	UFUNCTION(NetMulticast, Reliable)
 	void MulticastRPC_Attack01();

	void RetrunOriginLoc();

	UFUNCTION(Server, Reliable)
	void ServerRPC_TakeDamage();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_TakeDamage();

	UFUNCTION(Server, Reliable)
	void ServerRPC_GruxDie();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_GruxDie();

	UFUNCTION(Server, Reliable)
	void ServerRPC_GruxCameraShake();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_GruxCameraShake();

	UFUNCTION(Server, Reliable)
	void ServerRPC_GruxTakeDamageWidgetSet();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_GruxTakeDamageWidgetSet(int32 value2);

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class AEXPActor> expOrb;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class APickup> pickUpActor1;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class APickup> pickUpActor2;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class APickup> pickUpActor3;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class APickup> pickUpActor4;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class APickup> pickUpActor5;

	UFUNCTION(Server, Reliable)
	void ServerRPC_GruxDropExp();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_GruxDropExp();

};
