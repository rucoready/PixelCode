// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h" // ?
#include "Player/PlayerOrganism.h"
#include "Player/Interfaces/InteractionInterface.h" // cpp���ƴ� ����� �־���.
#include "PixelCodeCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UCombatComponent;
class APlayerHUD;
class UInventoryComponent;
class UItemBase;
class AItemStorage;
class UPlayerStatWidget;
class ABuilding;
class UNormallyWidget;
class ABaseWeapon;

UENUM()
enum class MyEnum : int8
{
	NM_Standalone,
	NM_DedicatedServer,
	NM_ListenServer,
	NM_Client,
	NM_MAX,
};

// ����s
USTRUCT()
struct FInteractionData
{	
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{

	}

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime; // ��ȣ�ۿ� �����׸� ����, ������ ��ȣ�ۿ� ����, �ð�Ȯ��

};
// ����e

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);




// ����-----------------------------------------------------------------------------------------------------
class ABuildingVisual;
// ����-----------------------------------------------------------------------------------------------------��



UCLASS(config=Game)
class APixelCodeCharacter : public APlayerOrganism
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleCombatAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Pressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_RollandRun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Stat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Crafting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_SkillQ;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_SkillE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_SkillR;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_SkillZ;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Skill_RightMouse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) 
	UInputAction* IA_SetBuildMode;
  
  	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) 
	UInputAction* IA_RemoveFoliage;

 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) 
	UInputAction* IA_SpawnBuilding; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) 
	UInputAction* IA_Weapon; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) 
	UInputAction* IA_Weapon2; 

public:
	APixelCodeCharacter();
	// ���� S
	FORCEINLINE bool IsInteracting() const {return GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_Interaction); }; // ���� ��ȣ�ۿ������� �ƴ���

	void UpdateInteractionWidget() const;

	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);

	class AInterfaceTestActor* InterfaceActor;

	UPROPERTY()
	UPlayerStatWidget* statWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UPlayerStatWidget> StatWidgetClass;

	UPROPERTY()
	UNormallyWidget* NormallyWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UNormallyWidget> NormallyWidgetClass;

	bool bIsStatVisible = true;

	// ������
	bool bRoll = false;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* RollAnim;

	float RollTime = 0;

	// ��ų
	void SkillQ();

	void SkillE();

	void SkillR();

	void SkillZ();

	void SkillRightMouse();
	
	FVector CachedDestination;
	void Mousehit();

	void switchWeapon();

	void switchWeapon2();

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NS_SkillQ;
	
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NS_SkillR;

	//void RollCharacterForward(APixelCodeCharacter* PixelCodeCharacter, float RollDistance);

	// ���� E
protected:
	// ���� S
	UPROPERTY()
	APlayerHUD* HUD;

	// ���� E
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	void CharacterJump(const FInputActionValue& Value);

	void LightAttackFunction(const FInputActionValue& Value);

	void ToggleCombatFunction(const FInputActionValue& Value);

	void PlayerRoll(const FInputActionValue& Value);

	void PlayerRun(const FInputActionValue& Value);

	void PlayerRunEnd(const FInputActionValue& Value);

	bool bInventorystate = false;

	virtual void Tick(float DeltaTime) override;

	void PrintInfo();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
		
	// To add mapping context
	virtual void BeginPlay();

	// ���� s
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	// ����-----------------------------------------------------------------------------------------------------
	FHitResult PerformLineTrace(float Distance = 650.0f, bool DrawDebug = false);

	UPROPERTY(BlueprintReadOnly, Category = KSH)
	bool bInBuildMode;

	UPROPERTY(Replicated,EditDefaultsOnly, Category = KSH)
	TSubclassOf<ABuildingVisual> BuildingClass;

	UPROPERTY(Replicated,EditDefaultsOnly, BlueprintReadOnly, Category = KSH)
	ABuildingVisual* Builder;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = KSH)
	ABuilding* Buildings;


	FVector BuildLoc;

	void SetBuildPosition(const FHitResult& HitResult);
	// ����-----------------------------------------------------------------------------------------------------��


	float InteractionCheckFrequecy;

	float InteractionCheckDistance; // ������ ĳ���Ϳ��� �󸶳� �ָ� �߻����

	FTimerHandle TimerHandle_Interaction; 

	UPROPERTY(Replicated)
	FInteractionData InteractionData;

	void ToggleMenu();

	void StatMenu();

	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable); // ��ȣ�ۿ� �����׸�ȣ��, �� ��ȣ�ۿ� �����׸� ��������
	void NoInteractableFound();// ������ ã�����ϸ� ȣ��X
	void BeginInteract();
	void EndInteract();
	// ���� e
		
	UFUNCTION(Server, Reliable)
	void ServerRPC_Interact();


	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_Interact(const TScriptInterface<IInteractionInterface>& Interactable);

	APlayerOrganism* focusedChar = nullptr;

	APixelCodeCharacter* self = this;

public:

	void Interact();

	// ����
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "camera", meta )

	UFUNCTION()
	void OnCraftingPressed();

	// ������ ���� �Լ�
	UFUNCTION()
	void CraftItem(const FCraftItem& Item);

	// ���� ǰ��
	AItemStorage* GetItemStorage();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = KYH)
	AItemStorage* ItemStorage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = KYH)
	TSubclassOf<AActor>	ItemStorageTemplate;


	// ����-----------------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = KSH)
	void SetBuildMode(bool Enabled);

	UFUNCTION(BlueprintCallable, Category = KSH)
	bool GetBuildMode() const { return bInBuildMode; }

	UFUNCTION(BlueprintCallable, Category = KSH)
	void CycleBuildingMesh();

	UFUNCTION(BlueprintCallable, Category = KSH)
	void SpawnBuilding();

	UFUNCTION(BlueprintCallable, Category = KSH)
	void DestroyBuildingInstance();

	UFUNCTION()
	void OnSetBuildModePressed();  

	UFUNCTION()
	void OnRemoveFoliagePressed();  

	UFUNCTION(BlueprintCallable, Category = KSH) 
	void RemoveFoliage(const FHitResult& HitResult);

	UFUNCTION(Server, Reliable)
	void ServerRPC_RemoveFoliage();  

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_RemoveFoliage(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category = KSH)
	void OnSpawnBuildingPressed();

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = KSH)
	void ServerRPC_SpawnBuilding();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = KSH)
	void NetMulticastRPC_SpawnBuilding();

	UPROPERTY(EditAnywhere, Category=KSH)
	TSubclassOf<class APickup> pickupItem;


	// ����-----------------------------------------------------------------------------------------------------��
	/*UPROPERTY(EditAnywhere, Category="MySettings")
	class UAnimMontage* rollMT;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "weapon")
	TSubclassOf<class ABaseWeapon> defaultWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "weapon")
	TSubclassOf<class ABaseWeapon> axe;

	ABaseWeapon* equipment;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// ī�޶� �ӵ�
    UPROPERTY(EditAnywhere, Category = Camera)
    float CameraLagSpeed;

	FVector CameraLoc;

	UFUNCTION(Server, Reliable)
	void ServerRPC_ToggleCombat();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_ToggleCombat();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	void GetHit(const FVector& ImpactPoint);

	virtual void DieFunction() override;


	virtual void CreateInventory() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
};


