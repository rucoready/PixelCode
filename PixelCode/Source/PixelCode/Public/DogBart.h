// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player/pixelPlayerState.h"
#include "DogBart.generated.h"

UCLASS()
class PIXELCODE_API ADogBart : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADogBart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UBoxComponent* damageBox;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class APlayerOrganism* Player;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ApplyDamageToTarget(AActor* OtherActor, float DamageAmount);

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class APlayerController* Pc;

	FTimerHandle Timerhandle_CooltimeDamageCollisionActive;

	FTimerHandle timerhandle_Destroy;

	bool dogBartDie = false;

	float currentHp;

	float maxHp = 30.0f;

	void DestroySelf();

	void DogBartTakeDamage(float Damage);

	UFUNCTION()
	void DamageCollisionActive();

	UFUNCTION()
	void DamageCollisionDeactive();

	UFUNCTION()
	void OnBeginOverlapDamageCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="MySettings")
	class USkeletalMeshComponent* tailMeshSM;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="MySettings")
	class USkeletalMeshComponent* maneMeshSM;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MySettings")
	class USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere, Category = "MySetting")
	class UBehaviorTree* tree;
	class APixelCodeCharacter* PixelCodeCharacter;
	class ApixelPlayerState* PlayerState;

	UBehaviorTree* GetBehaviorTree() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* meleeAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* meleeAttack2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* jumpAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* takeDamage1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* die;

	bool onceDie;

	UFUNCTION(Server, Reliable)
 	void ServerRPC_MeleeAttack();
 
 	UFUNCTION(NetMulticast, Reliable)
 	void MulticastRPC_MeleeAttack();

	UFUNCTION(Server, Reliable)
	void ServerRPC_JumpAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_JumpAttack();

	UFUNCTION(Server, Reliable)
	void ServerRPC_TakeDamage();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_TakeDamage();

	UFUNCTION(Server, Reliable)
	void ServerRPC_Die();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Die();
};
