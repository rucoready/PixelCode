// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BossSword.h"
#include "Components/ChildActorComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BossApernia.generated.h"

UCLASS()
class PIXELCODE_API ABossApernia : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossApernia();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UWidgetComponent* damageWidgetComponentl;

	bool bossDied = false;

	UBehaviorTree* GetBehaviorTree() const;

	class UAIPerceptionStimuliSourceComponent* StimulusSource;

	void SetupStimulusSource();

	//class UStaticMeshComponent* bossSwordComp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UNiagaraComponent* phaseShieldComponent;


	class UStaticMeshComponent* bossBackSwordComp;

	//UPROPERTY(EditAnywhere, Category="MySettings")
	//class ABossSword* bossSwordComp;

	//UPROPERTY(EditAnywhere, Category = "MySettings")
	//TSubclassOf<ABossSword> BossSwordActorClass;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UChildActorComponent* bossSwordComp;

	UFUNCTION()
	void SwordCollisionActive();

	UFUNCTION()
	void SwordCollisionDeactive();

	bool destroyOnce = false;

	FTimerHandle timerhandle_Destroy;
	FTimerHandle timerhandle_Destroy2;

	void DestroySelfOriginMesh();

	void DestroySelf();

	//UPROPERTY(EditAnywhere, Category = "MySettings")
	//class ABossSword* sword;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class ABossSword> bossSwordclass;

	//float bossMaxHP = 43000.0f;
	UPROPERTY(EditAnywhere, Category = "HP")
	float bossMaxHP = 100000.0f;
	float bossCurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* bossTakeDamageMT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* bossTakeDamageMT01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* bossTakeDamageMT2;

	bool bStiffness = false;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector counterHitKey;

	bool TaskCheckCounterHit = false;
	

	//////Boss Damage System
	UFUNCTION()
	void BossTakeDamage(float Damage);

	UFUNCTION()
	void BossFallDown();

	void RepocessBehaviorTree();

	void RepocessBehaviorTreeRe();

	UPROPERTY(EditAnywhere, Category="MySettings")
	class APlayerOrganism* Player;

	bool bBossAttackFallDownAttack = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UBehaviorTreeComponent* savedBTComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    FVector savedLocation;

	FTimerHandle timerhandle_RepocessBehaviorTree;

	void BossFallDownReset();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* FallDown;

	UPROPERTY(EditAnywhere, Category="MyMaterial")
	class UMaterial* originalMaterial;
	UPROPERTY(EditAnywhere, Category="MyMaterial")
	class UMaterial* originalMaterial2;

	

	UPROPERTY(EditAnywhere, Category="MyMaterial")
	class UMaterial* damageMaterial;

	UPROPERTY(EditAnywhere, Category="MyMaterial")
	class UMaterial* counterMaterial;

	UPROPERTY(EditAnywhere, Category="MyMaterial")
	class UMaterial* counterMaterial2;

	UPROPERTY(EditAnywhere, Category="MyMesh")
	USkeletalMeshComponent* meshComponent;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UNiagaraSystem* counterHitNA2;

	void SetOriginMaterial();

	void SetCounterMaterial();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* counterGroggy;

	bool bossHitCounterAttack = false;

	void ReflagCounterAttack();

	FTimerHandle timerhandle_ReflagCounterAttack;

	FTimerHandle timerhandle_SetOriginMatetrial;

	FTimerHandle timerhandle_BindSword;

	void RestoreBind();

	bool onceBindSword = false;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* counterSound; // 

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* fractureSound;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* gigantSwordSound;

	bool onceSoundGigantImpact = false;

	//////////////////////////UI


	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UBossMaInUI* bossMainWidget;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	TSubclassOf<class UBossMaInUI> MainUIFactory;

	void InitMainUI();

	///////////////////////////////////////////////////////////////////////////////////////////////Network//////////////////////////////////////////////////////

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	



	//Forward Slash//////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* swordComboAttack1;

	UFUNCTION(Server, Reliable)
	void ServerRPC_ForwardSlash();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ForwardSlash();

	//Jump Attack01/////////////////////////////////////////////////////////////////


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* jumpAttack01V1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* jumpAttack01V2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings" )
	class UAnimMontage* jumpAttack01V3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* jumpAttack01V4;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	class UNiagaraSystem* groundImpactV1Niagara;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	class UNiagaraSystem* bissHitNA;


	UFUNCTION(Server, Reliable)
	void ServerRPC_JumpAttack01V1();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_JumpAttack01V1();

	UFUNCTION(Server, Reliable)
	void ServerRPC_JumpAttack01V2();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_JumpAttack01V2();

	UFUNCTION(Server, Reliable)
	void ServerRPC_JumpAttack01V3();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_JumpAttack01V3();

	UFUNCTION(Server, Reliable)
	void ServerRPC_JumpAttack01V4();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_JumpAttack01V4();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnJumpAttackNiagara();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnJumpAttackNiagara();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnJumpAttackSwordPositionSet();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnJumpAttackSwordPositionSet();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnJumpAttackSwordPositionSet2();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnJumpAttackSwordPositionSet2();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnJumpAttackSwordPositionReSet();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnJumpAttackSwordPositionReSet();



	//Jump Attack02/////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* jumpAttack02V1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* jumpAttack02V2;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	class UNiagaraSystem* jumpSwing;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	class UNiagaraSystem* jumpSwing2;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	class UNiagaraSystem* jumpSwing3;

	UFUNCTION(Server, Reliable)
	void ServerRPC_JumpAttack02V1();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_JumpAttack02V1();

	UFUNCTION(Server, Reliable)
	void ServerRPC_JumpAttack02V2();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_JumpAttack02V2();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnJumpAttackNiagara2V1();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnJumpAttackNiagara2V1();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnJumpAttackNiagara2V2();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnJumpAttackNiagara2V2();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnJumpAttackNiagara2V3();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnJumpAttackNiagara2V3();

	//Jump Attack03/////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* jumpAttack03V1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* jumpAttack03V2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* jumpAttack03V3;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	class UNiagaraSystem* groundImpacts;

	UPROPERTY(EditAnywhere, Category = "Particles")
	class UParticleSystem* groundImpactParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	TSubclassOf<UCameraShakeBase> cameraShakeOBJ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	TSubclassOf<UCameraShakeBase> cameraShakeHitPlayerOBJ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	TSubclassOf<UCameraShakeBase> cameraShakeCounterOBJ;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class ABoundCollision> boundCollision;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class ABoundCollision> boundCollisionRoar;

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnBoundCollision();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnBoundCollision();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnBoundCollisionRoar();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnBoundCollisionRoar();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnBoundCollision2();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnBoundCollision2();

	UFUNCTION(Server, Reliable)
	void ServerRPC_JumpAttack03V1();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_JumpAttack03V1();

	UFUNCTION(Server, Reliable)
	void ServerRPC_JumpAttack03V2();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_JumpAttack03V2();

	UFUNCTION(Server, Reliable)
	void ServerRPC_JumpAttack03V3();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_JumpAttack03V3();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnJumpAttackNiagara3V1();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnJumpAttackNiagara3V1();

	UFUNCTION(Server, Reliable)
	void ServerRPC_JumpAttack03CameraShake();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_JumpAttack03CameraShake();

	UFUNCTION(Server, Reliable)
	void ServerRPC_JumpAttack03SwordPositionSet();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_JumpAttack03SwordPositionSet();

	UFUNCTION(Server, Reliable)
	void ServerRPC_JumpAttack03SwordPositionReSet();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_JumpAttack03SwordPositionReSet();

	//Normal Attack 01/////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* swordNormalAttack01V1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* swordNormalAttack01V2;

	UFUNCTION(Server, Reliable)
	void ServerRPC_NormalAttack01V1();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_NormalAttack01V1();

	UFUNCTION(Server, Reliable)
	void ServerRPC_NormalAttack01V2();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_NormalAttack01V2();

	//Normal Attack 02/////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* swordNormalAttack02V1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* swordNormalAttack02V2;

	UFUNCTION(Server, Reliable)
	void ServerRPC_NormalAttack02V1();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_NormalAttack02V1();

	UFUNCTION(Server, Reliable)
	void ServerRPC_NormalAttack02V2();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_NormalAttack02V2();

	//Normal Attack 03/////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* swordNormalAttack04;

	UFUNCTION(Server, Reliable)
	void ServerRPC_NormalAttack03V1();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_NormalAttack03V1();

	//Boss Forward Slash/////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* forwardSlashAttack;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	class UNiagaraSystem* impactSwing;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	class UNiagaraSystem* impactSwing2;

	UFUNCTION(Server, Reliable)
	void ServerRPC_ForwardSlashAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ForwardSlasAttack();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnNiagaraForwardSlash();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnNiagaraForwardSlash();

	//Dodge Add Slash/////////////////////////////////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* dodgeAndAttack01;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	class UNiagaraSystem* doubleSwingNA2;

	UFUNCTION(Server, Reliable)
	void ServerRPC_DodgeAndSlashAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DodgeAndSlashAttack();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnNiagaraDodgeAndSlash();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnNiagaraDodgeAndSlash();

	//Double Swing/////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UAnimMontage* doubleSwingAttack;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	class UNiagaraSystem* doubleSwingV2;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	class UNiagaraSystem* doubleSwingV3;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<UCameraShakeBase> cameraShakeOBJ2;

	UFUNCTION(Server, Reliable)
	void ServerRPC_DoubleSwingAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DoubleSwingAttack();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnNiagaraDoubleSwing01();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnNiagaraDoubleSwing01();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnNiagaraDoubleSwing02();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnNiagaraDoubleSwing02();

	UFUNCTION(Server, Reliable)
	void ServerRPC_DoubleSwingCameraShake();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DoubleSwingCameraShake();

	//Dodge01 [Right]/////////////////////////////////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* dodge01;

	UFUNCTION(Server, Reliable)
	void ServerRPC_DodgeRight();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DodgeRight();

	//Dodge02 [Left]/////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* dodge02;

	UFUNCTION(Server, Reliable)
	void ServerRPC_DodgeLeft();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DodgeLeft();

	//Dodge03 [Back]/////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* dodge03;

	UFUNCTION(Server, Reliable)
	void ServerRPC_DodgeBack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DodgeBack();

	//Sting Attack/////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* stingAttack01;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	class UNiagaraSystem* stingAttackNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	TSubclassOf<UCameraShakeBase> cameraShakeStingOBJ;



	UFUNCTION(Server, Reliable)
	void ServerRPC_StingAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_StingAttack();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnNiagaraStingAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnNiagaraStingAttack();

	UFUNCTION(Server, Reliable)
	void ServerRPC_StingAttackCameraShake();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_StingAttackCameraShake();

	UFUNCTION(Server, Reliable)
	void ServerRPC_StingAttackSwordPositionSet();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_StingAttackSwordPositionSet();

	UFUNCTION(Server, Reliable)
	void ServerRPC_StingAttackSwordPositionReSet();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_StingAttackSwordPositionReSet();

	//Far Jump Attack/////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* farStompAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	TSubclassOf<UCameraShakeBase> cameraShakeFarStompAttackOBJ;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UParticleSystem* groundImpactParticle2;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UNiagaraSystem* groundImpactNiagara2;

	UFUNCTION(Server, Reliable)
	void ServerRPC_FarJumpAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_FarJumpAttack();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnNiagaraFarJumpAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnNiagaraFarJumpAttack();


	UFUNCTION(Server, Reliable)
	void ServerRPC_FarJumpAttackCameraShake();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_FarJumpAttackCameraShake();

	UFUNCTION(Server, Reliable)
	void ServerRPC_FarJumpAttackSwordPositionSet();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_FarJumpAttackSwordPositionSet();

	UFUNCTION(Server, Reliable)
	void ServerRPC_FarJumpAttackSwordPositionReSet();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_FarJumpAttackSwordPositionReSet();

	//Boss First Equip Sword Scene/////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* swordEquipMT;

	UFUNCTION(Server, Reliable)
	void ServerRPC_SwordFirstEquip();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SwordFirstEquip();

	//Boss Falldown//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* bossFallDownMT;

	UFUNCTION(Server, Reliable)
	void ServerRPC_BossFallDown(float Damage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_BossFallDown(float Damage);

	//Boss CounterPrecursor

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* counterPrecursor;

	UPROPERTY(EditAnywhere, Category = "Particles")
	class UParticleSystem* counterImpactParticle;

	UFUNCTION(Server, Reliable)
	void ServerRPC_CounterPrecursor();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_CounterPrecursor();

	UFUNCTION(Server, Reliable)
	void ServerRPC_CounterPrecursorSpawnParticle();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_CounterPrecursorSpawnParticle();

	//Boss Die
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* bossDie;

	UFUNCTION(Server, Reliable)
	void ServerRPC_BossDie();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_BossDie();

	bool bossOnceDie = false;

	//damageWidget
	int32 widgetRandomValue;

	class UDamageWidget* damageWidgetInstance;

	int32 damageAmount;

	UFUNCTION(Server, Reliable)
	void ServerRPC_BossTakeDamageWidgetSet();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_BossTakeDamageWidgetSet(int32 value2);

	///////////////phase2

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* bossRoar;

	UFUNCTION(Server, Reliable)
	void ServerRPC_Boss2phaseRoar();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Boss2phaseRoar();

	UPROPERTY(EditAnywhere, Category = "Particles")
	class UParticleSystem* roarParticle;


	UPROPERTY(EditAnywhere, Category = "Niagara")
	class UNiagaraSystem* phaseShield;

	UFUNCTION(Server, Reliable)
	void ServerRPC_RoarParticle();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_RoarParticle();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnGigantSword();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnGigantSword();

	UFUNCTION(Server, Reliable)
	void ServerRPC_MoveGigantSword();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_MoveGigantSword();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class APhase2GigantSword> gigantSword;

	void SpawnGigantSword();

	void MoveGigantSword();

	UFUNCTION(Server, Reliable)
	void ServerRPC_GigantSwordCameraShake();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_GigantSwordCameraShake();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	TSubclassOf<UCameraShakeBase> gigantSwordCameraShake;

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnDecalSword();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnDecalSword();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnDecalSword2();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnDecalSword2();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnDecalSword3();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnDecalSword3();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnDecalSword4();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnDecalSword4(const TArray<FVector>& decalLocations4);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnDecalSword5();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnDecalSword5(const TArray<FVector>& decalLocations);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnDecalSword6();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnDecalSword6();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnDecalSword7();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnDecalSword7();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnDemonSword2Phase();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnDemonSword2Phase(const TArray<FTransform>& SpawnTransformsD);


	void SpawnSwordDelay();

	UFUNCTION(Server, Reliable)
	void ServerRPC_lastSpawnDecalSword1();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_lastSpawnDecalSword1();

	UFUNCTION(Server, Reliable)
	void ServerRPC_lastSpawnDecalSword2();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_lastSpawnDecalSword2();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnLazorDragonStatue();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnLazorDragonStatue();

	float currentTime;

	void SpawnDecal(int32 Index);

	// 데칼 스폰을 위한 타이머 핸들
	FTimerHandle TimerHandle;

	TArray<FVector> savedSpawnLocations;
	TArray<FTimerHandle> timerhandle_FinalDecalSpawn;
	TSubclassOf<AActor> savedDecalClass;

	// 현재 스폰할 데칼의 인덱스
	int CurrentDecalIndex;

	// 스폰 위치와 변환 정보를 저장할 배열
	TArray<FTransform> SpawnTransforms;


	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class AWarningCircleDecal> decalCircle;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class ADemonSword> demonSwordV2;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class ADragonRazorStatue> dragonStatue;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class AFireActor> fireActor;

	class ADragonRazorStatue* drangonStatue;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	class UNiagaraSystem* fireNA;

	bool allStatueDestroy = false;

	bool onceRestore2PhaseBoss = false;

	UFUNCTION(Server, Reliable)
	void ServerRPC_CheckingStatueSurvive();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_CheckingStatueSurvive();

	AActor* spawnStatue1;

	AActor* spawnStatue2;

	AActor* spawnStatue3;

	int statueDestroyCount;


	class UMaterialInstanceDynamic* DynamicMaterialInstance;

	UPROPERTY(EditAnywhere, Category = "MyMaterial")
	class UMaterial* stoneMaterial;

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnStoneDecal();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnStoneDecal();

	UFUNCTION(Server, Reliable)
	void ServerRPC_DropBossExp();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DropBossExp();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class AEXPActor> expOrb;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class ABossStoneChangeDecal> stoneDecal;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class AFractureBoss> fractureBoss;

	void SpawnFractureBoss();

	AActor* spawnDecalActor;

	AActor* fractureBossMesh;

	FTimerHandle timerhandle_SpawnFractureBoss;

	FTimerHandle timerhandle_DestroyDecalActor;

	void DestroyStoneDecal();

	class UCapsuleComponent* CapsuleComponent;

	class UPrimitiveComponent* RootComponent;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<class AGeometryCollectionActor> FractureBossClass;

	
	class AGeometryCollectionActor* fractureBossMesh2;

	UFUNCTION(Server, Reliable)
	void ServerRPC_Boss2phaseGoUp();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Boss2phaseGoUp();

	UFUNCTION(Server, Reliable)
	void ServerRPC_DestroyShield();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DestroyShield();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnAllDestroyActor();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnAllDestroyActor();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnGoldBossStatue();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnGoldBossStatue();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class APickup> goldBossStatue;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class ADestroyAllActor> destroyActor;

	bool onceShield = false;

	bool onceDestroyActorSpawn = false;

	float lerpDuration = 3.0f;

	bool bIsMovingUp = false;

	bool bHasReachedTarget = false;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector targetLocationPhase2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float movementDurationPhase2;

	FVector startLocationPhase2;
	FVector endLocationPhase2;
	bool bIsMovingPhase2;
	float moveDurationPhase2;
	float elapsedTimePhase2;

	FTimerHandle movementTimerHandle;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class APortalCollision> exitBackGate;

	class APortalCollision* portalGate;

	
	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class AMyMapCollsionActor> endPortal;

	class AMyMapCollsionActor* myMapCollsionActor;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class ABossFloor> floor;

	class ABossFloor* floorActor;

	
};
