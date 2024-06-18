// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerOrganism.h"
#include <Engine/DamageEvents.h>
#include <Kismet/KismetSystemLibrary.h>
#include "Animation/AnimMontage.h"
#include "Player/BaseWeapon.h"
#include "Player/CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
//#include "LootPanel.h"
#include "Player/inventory/InventoryComponent.h"
#include "Player/Interfaces/InventoryPanel.h"
#include "Player/World/Pickup.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>


// Sets default values
APlayerOrganism::APlayerOrganism()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	stateComp = CreateDefaultSubobject<UStateComponent>(TEXT("StateComp"));

	combatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));

	characterName = TEXT("Default");

	CreateInventory();
}

// Called when the game starts or when spawned
void APlayerOrganism::BeginPlay()
{
	Super::BeginPlay();

	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("characterName : %s"), *characterName));

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	stateComp->InitStat();
	stateComp->UpdateStat();
	stateComp->dieDelegate.BindUFunction(this, FName("DieFunction"));
}

// Called every frame
void APlayerOrganism::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerOrganism::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float APlayerOrganism::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float temp = stateComp->AddStatePoint(HP, -DamageAmount);

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
	}

	// 디버그
	if (0)
	{
		if (EventInstigator != nullptr)
		{
			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("EventInstigator : %s"), *EventInstigator->GetActorNameOrLabel()));
		}

		if (DamageCauser != nullptr)
		{
			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Attack By : %s"), *DamageCauser->GetActorNameOrLabel()));
		}
	}

	return 0.0f;
}

void APlayerOrganism::ContinueAttack_Implementation()
{
	if (combatComponent == nullptr) return;

	combatComponent->bAttacking = false;

	if (combatComponent->bAttackSaved)
	{
		combatComponent->bAttackSaved = false;

		AttackEvent();
	}
}

void APlayerOrganism::ResetCombat_Implementation()
{
	motionState = ECharacterMotionState::Idle;
	combatComponent->attackCount = 0;
}

bool APlayerOrganism::CanReceiveDamage_Implementation()
{
	return !bDead;
}

void APlayerOrganism::AttackEvent()
{
	if (combatComponent == nullptr) return;

	if (motionState == ECharacterMotionState::Idle || motionState == ECharacterMotionState::Attack)
	{
		PerformAttack(combatComponent->attackCount, false);
	}
}

void APlayerOrganism::PerformAttack(int32 attackIndex, bool bUseRandom)
{
	ABaseWeapon* mainWeapon = nullptr;
	UAnimMontage* useMontage = nullptr;

	if (combatComponent)
	{
		mainWeapon = combatComponent->GetMainWeapon();

		if (mainWeapon != nullptr)
		{
			// 랜덤 인덱스 생성
			int32 montagesSize = mainWeapon->attackMontages.Num();
			int32 randIndex = FMath::RandRange(0, montagesSize - 1);

			useMontage = bUseRandom ? mainWeapon->attackMontages[randIndex] : mainWeapon->attackMontages[attackIndex];

			if (IsValid(useMontage))
			{
				ServerRPC_PerformAttack(useMontage);
			}
		}
	}
}

void APlayerOrganism::EnableRagdoll()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None, 0);

		if (GetCapsuleComponent())
		{
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

			FAttachmentTransformRules attachTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);

			if (GetMesh())
			{
				GetMesh()->SetCollisionProfileName(TEXT("ragdoll"));
				GetMesh()->SetAllBodiesBelowSimulatePhysics(pelvisBoneName, true, true);
				GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(pelvisBoneName, 1.0f, false, true);
			}

			//if (CameraBoom)
			//{
			//	if (GetMesh())
			//	{
			//		CameraBoom->AttachToComponent(GetMesh(), attachTransformRules, pelvisBoneName);
			//		CameraBoom->bDoCollisionTest = false;

			//		GetMesh()->SetCollisionProfileName(TEXT("ragdoll"));
			//		GetMesh()->SetAllBodiesBelowSimulatePhysics(pelvisBoneName, true, true);
			//		GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(pelvisBoneName, 1.0f, false, true);
			//	}
			//	else
			//	{
			//		UE_LOG(LogTemp, Warning, TEXT("AEromCharacter : %d"), __LINE__);
			//	}
			//}
			//else
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("AEromCharacter : %d"), __LINE__);
			//}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AEromCharacter : %d"), __LINE__);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AEromCharacter : %d"), __LINE__);
	}
}

void APlayerOrganism::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerOrganism, stateComp);
	DOREPLIFETIME(APlayerOrganism, combatComponent);
	DOREPLIFETIME(APlayerOrganism, motionState);
	DOREPLIFETIME(APlayerOrganism, bDead);
	//DOREPLIFETIME(APlayerOrganism, itemRandNums);
	//DOREPLIFETIME(APlayerOrganism, randItemIndex);

}

void APlayerOrganism::ServerRPC_PerformAttack_Implementation(UAnimMontage* useMontage)
{
	ABaseWeapon* mainWeapon = combatComponent->GetMainWeapon();

	combatComponent->bAttacking = true;

	motionState = ECharacterMotionState::Attack;

	// 여기나 다른데서 캐릭터 스탯 및 무기 데미지 등 적용
	mainWeapon->weaponDamage = mainWeapon->weaponDamage;

	// 카운트 증가
	combatComponent->attackCount++;

	int32 montageLastIndex = mainWeapon->attackMontages.Num() - 1;

	// 카운트 초기화
	if (combatComponent->attackCount > montageLastIndex)
	{
		combatComponent->attackCount = 0;
	}

	NetMulticastRPC_PerformAttack(useMontage);
}

void APlayerOrganism::NetMulticastRPC_PerformAttack_Implementation(UAnimMontage* useMontage)
{
	float attackAnimTime = PlayAnimMontage(useMontage);

	FTimerHandle handler;
	GetWorldTimerManager().SetTimer(handler, [&]() {

		combatComponent->bAttacking = false;
		motionState = ECharacterMotionState::Idle;

		GetWorldTimerManager().ClearTimer(handler);

		}, 1.0f, false, attackAnimTime);
}

void APlayerOrganism::ServerRPC_AmountDamage_Implementation(float damage)
{
	if (stateComp != nullptr)
	{
		stateComp->AddStatePoint(HP, -damage);
	}
}

void APlayerOrganism::NetMulticastRPC_AmountDamage_Implementation(float damage)
{
	if (stateComp != nullptr)
	{
		stateComp->AddStatePoint(HP, -damage);
	}
}

void APlayerOrganism::DieFunction()
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Dead : %s"), *GetActorNameOrLabel()));

	bDead = true;

}

void APlayerOrganism::LootByOthers(APlayerOrganism* otherCharacter)
{
	//if (motionState != ECharacterMotionState::Die)
	//	return;

	//otherCharacter->GetController();

	//auto panel = Cast<UInventoryPanel>(otherCharacter->lootPanelWidget->GetWidgetFromName(FName(TEXT("WBP_InventoryPanel"))));

	//if (panel)
	//{
	//	panel->SetTargetCharacter(this);
	//	otherCharacter->lootPanelWidget->AddToViewport();
	//}
}

void APlayerOrganism::CreateInventory()
{
	if (PlayerInventory == nullptr)
	{
		PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	}

	PlayerInventory->SetSlotsCapacity(20); //인벤토리 슬롯 20개생성
	PlayerInventory->SetWeightCapacity(50.0f); // 무게용량 50설정
}

//void APlayerOrganism::InitRandomItem()
//{
//	itemRandNums = FMath::RandRange(5, 10);
//
//	ServerRPC_SetItemRandNums(itemRandNums);
//
//	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Init randNums : %d"), itemRandNums));
//	for (int i = 0; i < itemRandNums; i++)
//	{
//		randItemIndex = FMath::RandRange(1, 6);
//
//		ServerRPC_SetItemRandIndex(randItemIndex);
//
//		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Add Item : test_%03d"), randItemIndex));
//
//		ServerRPC_SpawnItem(randItemIndex);
//	}
//
//	PlayerInventory->RefreshInventory();
//}

void APlayerOrganism::ServerRPC_SetItemRandNums_Implementation(int32 randNums)
{
	itemRandNums = randNums;

	NetMulticastRPC_SetItemRandNums(randNums);
}

void APlayerOrganism::NetMulticastRPC_SetItemRandNums_Implementation(int32 randNums)
{
	itemRandNums = randNums;
}

void APlayerOrganism::ServerRPC_SetItemRandIndex_Implementation(int32 randIndex)
{
	randItemIndex = randIndex;

	NetMulticastRPC_SetItemRandIndex(randIndex);
}

void APlayerOrganism::NetMulticastRPC_SetItemRandIndex_Implementation(int32 randIndex)
{
	randItemIndex = randIndex;
}

void APlayerOrganism::ServerRPC_SpawnItem_Implementation(int32 itemIndex)
{
	NetMulticastRPC_SpawnItem(itemIndex);
}

void APlayerOrganism::NetMulticastRPC_SpawnItem_Implementation(int32 randIndex)
{
	FActorSpawnParameters spawnParam;
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), FTransform(), spawnParam);
	FString strName = FString::Printf(TEXT("test_%03d"), randItemIndex);
	pickup->DesiredItemID = FName(*strName);
	pickup->SetInput(this);
}

