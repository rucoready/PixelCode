// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h" // ?
#include "Player/PlayerOrganism.h"
#include "Player/Interfaces/InteractionInterface.h" // cpp가아닌 헤더에 둬야함.
#include "Player/inventory/ItemBase.h"
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
class ABuildingVisual;
class ABuilding;
class UNormallyWidget;
class ABaseWeapon;
class UCreateItemData;
class AParentItem;
struct FRecipe;
class ASpawnSwordQSkill;

UENUM()
enum class MyEnum : int8
{
	NM_Standalone,
	NM_DedicatedServer,
	NM_ListenServer,
	NM_Client,
	NM_MAX,
};

// 진원s
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
	float LastInteractionCheckTime; // 상호작용 가능항목 포함, 마지막 상호작용 포함, 시간확인

};
// 진원e

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

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
	UInputAction* IA_CycleMesh; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) 
	UInputAction* IA_DestroyBuilding; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) 
	UInputAction* IA_Weapon; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) 
	UInputAction* IA_Weapon2; 

public:
	APixelCodeCharacter();
	// 진원 S
	FORCEINLINE bool IsInteracting() const {return GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_Interaction); }; // 현재 상호작용중인지 아닌지

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

	// 구르기
	bool bRoll = false;

	
	UPROPERTY(Replicated,EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* RollAnim;

	float RollTime = 0;

	// 스킬
	void SkillQ();

	void SkillE();

	void SkillR();

	void SkillZ();

	void SkillRightMouse();

	// ��ų ��Ÿ��

	bool bQskillCoolTime = false;
	bool bEskillCoolTime = false;
	bool bRskillCoolTime = false;
	bool bZskillCoolTime = false;
	void QskillTime();
	void EskillTime();
	void RskillTime();
	void ZskillTime();

	FTimerHandle QSkillTimer;
	FTimerHandle ESkillTimer;
	FTimerHandle RSkillTimer;
	FTimerHandle ZSkillTimer;

	float QSkillCoolTime = 6;
	float ESkillCoolTime = 10;
	float RSkillCoolTime = 8;
	float ZSkillCoolTime = 20;

	float CurrentQSkillCoolTime = 0;
	float CurrentESkillCoolTime = 0;
	float CurrentRSkillCoolTime = 0;
	float CurrentZSkillCoolTime = 0;

	UPROPERTY()
	ASpawnSwordQSkill* SpawnQSkillCollsion;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<ASpawnSwordQSkill> QSkillSpawn;


	FVector CachedDestination;

	void Mousehit();

	void switchWeapon();

	void switchWeapon2();

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NS_SkillQ;
	
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NS_SkillR;

	//void RollCharacterForward(APixelCodeCharacter* PixelCodeCharacter, float RollDistance);

	// 진원 E
protected:
	// 진원 S
	UPROPERTY()
	APlayerHUD* HUD;

	// 진원 E
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	void CharacterJump(const FInputActionValue& Value);

	void LightAttackFunction(const FInputActionValue& Value);

	void ToggleCombatFunction(const FInputActionValue& Value);

	void PlayerRoll(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerRoll();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerRoll();

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

	// 진원 s
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	// 서휘-----------------------------------------------------------------------------------------------------
	FHitResult PerformLineTrace(float Distance = 650.0f, bool DrawDebug = false);

	UPROPERTY(Replicated, BlueprintReadOnly, Category = KSH)
	bool bInBuildMode;

	UPROPERTY(Replicated,EditDefaultsOnly, Category = KSH)
	TSubclassOf<ABuildingVisual> BuildingClass;

	UPROPERTY(Replicated,EditDefaultsOnly, Category = KSH)
	TSubclassOf<ABuilding> BuildingC;

	UPROPERTY(Replicated,EditDefaultsOnly, BlueprintReadOnly, Category = KSH)
	ABuildingVisual* Builder;
	
	// 서휘-----------------------------------------------------------------------------------------------------끝


	float InteractionCheckFrequecy;

	float InteractionCheckDistance; // 추적이 캐릭터에서 얼마나 멀리 발사될지

	FTimerHandle TimerHandle_Interaction; 

	UPROPERTY(Replicated)
	FInteractionData InteractionData;

	void ToggleMenu();

	void StatMenu();

	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable); // 상호작용 가능항목호출, 새 상호작용 가능항목 가져오기
	void NoInteractableFound();// 하지만 찾지못하면 호출X
	void BeginInteract();
	void EndInteract();
	// 진원 e
		
	UFUNCTION(Server, Reliable)
	void ServerRPC_Interact();


	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_Interact(const TScriptInterface<IInteractionInterface>& Interactable);

	APlayerOrganism* focusedChar = nullptr;

	APixelCodeCharacter* self = this;

public:

	// 카메라 조절
	void CheckObstacles();

	void Interact();

	// 요한
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "camera", meta )

	UFUNCTION()
	void OnCraftingPressed();

	// 아이템 생성 함수
	UFUNCTION()
	void CraftItem(const FCraftItem& Item);

	UFUNCTION()
	void DropedItem(const UItemBase* Iteminfo);

	TArray<UItemBase*> GetInventory() const;
	
	// 저장 품목
	AItemStorage* GetItemStorage();

	// 아이템 테스트 들감
	UPROPERTY(EditAnywhere, Category = "KYH")
	class UItemBase* Iteminfos;

	UPROPERTY(EditAnywhere, Category = "KYH")
	class APickup* PickupItems;

	UPROPERTY(EditAnywhere, Category = "KYH")
	TArray<UItemBase*> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = KYH)
	AItemStorage* ItemStorage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = KYH)
	TSubclassOf<AActor>	ItemStorageTemplate;

	uint32 GetSpecificItemAmount(EItemName ItemName);

	void ReduceRecipeFromInventory(const TArray<FRecipe>& Recipes);

	UPROPERTY(EditAnywhere, Category = "Inventory")
	uint8 MaxInventorySlot;

	UPROPERTY()
	UInventoryComponent* OwningInventory; // 인벤토리

	// 서휘-----------------------------------------------------------------------------------------------------
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = KSH)
	ABuilding* Buildings;

	UFUNCTION(BlueprintCallable, Category = KSH)
	void SetBuildMode(bool Enabled);

	UFUNCTION(Server, Reliable)
 	void ServerRPC_SetBuildMode();

	UFUNCTION(Client, Reliable)
	void ClientRPC_SetBuildMode();

	UFUNCTION(BlueprintCallable, Category = KSH)
	bool GetBuildMode() const { return bInBuildMode; }

	UFUNCTION()
	void OnCycleMeshPressed();

	UFUNCTION(BlueprintCallable, Category = KSH)
	void CycleBuildingMesh();

	UFUNCTION(Server, Reliable)
 	void ServerRPC_CycleBuildingMesh();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_CycleBuildingMesh(UStaticMesh* newMesh);

	UFUNCTION(Client, Reliable)
	void ClientRPC_CycleBuildingMesh(UStaticMesh* newMesh);

	UFUNCTION(BlueprintCallable, Category = KSH)
	void SpawnBuilding();

	UFUNCTION()
	void OnDestroyBuildingPressed();

	UFUNCTION(BlueprintCallable, Category = KSH)
	void DestroyBuildingInstance(const FHitResult& HitResult);

	UFUNCTION(Server, Reliable)
	void ServerRPC_DestroyBuildingInstance();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_DestroyBuildingInstance(const FHitResult& HitResult);

// 	UFUNCTION(NetMulticast, Reliable)
// 	void NetMulticastRPC_DestroyBuildingInstance(UInstancedStaticMeshComponent* comp, const int32 index);
	
// 	UFUNCTION(NetMulticast, Reliable)
// 	void NetMulticastRPC_DestroyBuildingInstance(const FHitResult& HitResult);

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

	UFUNCTION()
	void OnSpawnBuildingPressed();

 	UFUNCTION(Server, Reliable)
 	void ServerRPC_SpawnBuilding();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_SpawnBuilding (EBuildType BuildType, FTransform transf);
	
	UPROPERTY(EditAnywhere, Category=KSH)
	TSubclassOf<class APickup> pickupItem;


	// 서휘-----------------------------------------------------------------------------------------------------끝
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

	// 카메라 속도
    UPROPERTY(EditAnywhere, Category = Camera)
    float CameraLagSpeed;

	FVector CameraLoc;

	UFUNCTION(Server, Reliable)
	void ServerRPC_ToggleCombat();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_ToggleCombat();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual void DieFunction() override;


	virtual void CreateInventory() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	private:
		FVector camPosition = FVector(-500, 0, 60);
};


