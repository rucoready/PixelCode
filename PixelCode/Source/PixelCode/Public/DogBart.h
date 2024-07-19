// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="MySettings")
	class USkeletalMeshComponent* tailMeshSM;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="MySettings")
	class USkeletalMeshComponent* maneMeshSM;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MySettings")
	class USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere, Category = "MySetting")
	class UBehaviorTree* tree;

	UBehaviorTree* GetBehaviorTree() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* meleeAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* meleeAttack2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* jumpAttack;

	UFUNCTION(Server, Reliable)
 	void ServerRPC_MeleeAttack();
 
 	UFUNCTION(NetMulticast, Reliable)
 	void MulticastRPC_MeleeAttack();

	UFUNCTION(Server, Reliable)
	void ServerRPC_JumpAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_JumpAttack();
};
