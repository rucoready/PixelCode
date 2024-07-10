// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/PixelCodeCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Player/PixelCodeGameInstance.h"
#include "Player/BaseWeapon.h"
#include "Player/CombatComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "Player/Interfaces/PlayerHUD.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include "Player/StateComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/EngineBaseTypes.h>
#include "DrawDebugHelpers.h" // 디버그라인
#include <../../../../../../../Source/Runtime/Core/Public/Math/UnrealMathUtility.h>
#include "Player/World/Pickup.h"
#include "Player/inventory/itemBase.h"
#include "Player/Inventory/InventoryComponent.h"
#include "Player/World/InterfaceTestActor.h"
//#include "LootPanel.h"
#include <../../../../../../../Source/Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h>
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Player/Interfaces/InteractionInterface.h"
#include <../../../../../../../Source/Runtime/CoreUObject/Public/UObject/ScriptInterface.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "BuildingVisual.h"
#include "ItemStorage.h"
#include "Player/PlayerStatWidget.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include <../../../../../../../Source/Runtime/Foliage/Public/FoliageInstancedStaticMeshComponent.h>
#include "Building.h"
#include "Player/Widget/NormallyWidget.h"
#include "../../../FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include "Player/World/Pickup.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APixelCodeCharacter

APixelCodeCharacter::APixelCodeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	CreateInventory();

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//FollowCamera->SetRelativeLocation(FVector(0, 0, 0));
	//FollowCamera->SetupAttachment(RootComponent);
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	characterName = TEXT("Player");

	InteractionCheckFrequecy = 0.1; // 빈도확인
	InteractionCheckDistance = 225.0f;

	BaseEyeHeight = 74.0f; // 플레이어 눈 높이위로

	//lootPanelWidget = CreateWidget<ULootPanel>(GetWorld(), ConstructorHelpers::FClassFinder<ULootPanel>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Enemy/UserInterfaces/WBP_LootPanel.WBP_LootPanel_C'")).Class);
	////lootPanelWidget->SetVisibility(ESlateVisibility::Collapsed);

	int iTemp = 0;

	// 서휘-----------------------------------------------------------------------------------------------------
	bInBuildMode = false;

	// 서휘-----------------------------------------------------------------------------------------------------끝
	
}

void APixelCodeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 10.0f;

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	FActorSpawnParameters spawnParam;
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParam.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	spawnParam.Owner = this;
	spawnParam.Instigator = this;

	equipment = GetWorld()->SpawnActor<ABaseWeapon>(defaultWeapon, GetActorTransform(), spawnParam);

	if (equipment)
	{
		equipment->OnEquipped();
	}

	HUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	InterfaceActor = Cast<AInterfaceTestActor>(InterfaceActor);

	/*if (IsLocallyControlled())
	{
		InitRandomItem();
	}*/

	//PlayerInventory->HandleAddItem();


	
	// 서휘-----------------------------------------------------------------------------------------------------
	if (BuildingClass)
	{
		Builder = GetWorld()->SpawnActor<ABuildingVisual>(BuildingClass);
	}
	// 서휘-----------------------------------------------------------------------------------------------------끝

	statWidget = CreateWidget<UPlayerStatWidget>(GetWorld(), StatWidgetClass);
	if (StatWidgetClass)
	{
		statWidget->AddToViewport(1);
		statWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	NormallyWidget = CreateWidget<UNormallyWidget>(GetWorld(), NormallyWidgetClass);
	if (NormallyWidgetClass)
	{
		NormallyWidget->AddToViewport(2);
		NormallyWidget->SetVisibility(ESlateVisibility::Visible);
	}

	// 요한----------------------------------------------------------------------

	ItemStorage = GetWorld()->SpawnActor<AItemStorage>(ItemStorageTemplate, FVector::ZeroVector, FRotator::ZeroRotator);



}

// 서휘-----------------------------------------------------------------------------------------------------
void APixelCodeCharacter::PerformLineTrace(float Distance , bool DrawDebug)
{
	ServerRPC_PerformLineTrace(Distance, DrawDebug);


	//if (DrawDebug)
	//{
	//	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	//}

}

void APixelCodeCharacter::ServerRPC_PerformLineTrace_Implementation(float Distance, bool DrawDebug)
{
	FVector Start = GetFollowCamera()->GetComponentLocation();
	FVector End = Start + GetFollowCamera()->GetForwardVector() * Distance;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
	NetMulticastRPC_PerformLineTrace(HitResult, Distance, DrawDebug);

	//NetMulticastRPC_PerformLineTrace(Distance,DrawDebug);
	
}

void APixelCodeCharacter::NetMulticastRPC_PerformLineTrace_Implementation(const FHitResult& HitResult, float Distance, bool DrawDebug)
{
	//PerformLineTrace(Distance, DrawDebug);
	
	Builder->ServerRPC_SetBuildPosition(HitResult);
	//HitResult?
}

// 서휘-----------------------------------------------------------------------------------------------------끝

void APixelCodeCharacter::ServerRPC_ToggleCombat_Implementation()
{
	motionState = ECharacterMotionState::ToggleCombat;

	combatComponent->bCombatEnable = !combatComponent->bCombatEnable;

	NetMulticastRPC_ToggleCombat();
}

void APixelCodeCharacter::NetMulticastRPC_ToggleCombat_Implementation()
{
	auto mainWeaponPtr = combatComponent->GetMainWeapon();

	float animPlayTime = 0.0f;

	if (!combatComponent->bCombatEnable)
	{
		if (mainWeaponPtr->exitCombatMontage)
		{
			animPlayTime = PlayAnimMontage(mainWeaponPtr->exitCombatMontage, 1.5f);

			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("animPlayTime : %f"), animPlayTime));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ToggleCombatFunction : %d"), __LINE__);
		}
	}
	else
	{
		if (mainWeaponPtr->enterCombatMontage)
		{
			animPlayTime = PlayAnimMontage(mainWeaponPtr->enterCombatMontage, 1.5f);

			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("animPlayTime : %f"), animPlayTime));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ToggleCombatFunction : %d"), __LINE__);
		}
	}

	//<< SSK 이거 먹히는지 테스트는 해봐야 됨
	FTimerHandle timerHandle;

	GetWorldTimerManager().SetTimer(timerHandle, [&]()
		{
			motionState = ECharacterMotionState::Idle;

			GetWorld()->GetTimerManager().ClearTimer(timerHandle);

			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("combatComponent->bCombatEnable : %s"), combatComponent->bCombatEnable ? TEXT("TRUE") : TEXT("FALSE")));

		}, animPlayTime, false, 1.0f);
}

void APixelCodeCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{ GetPawnViewLocation() }; // 명시적 초기화, 대괄호
	FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) }; // 마우스로 보기로 변경

	float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector()); // 내적함수

	if (LookDirection > 0)
	{
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FHitResult TraceHit; // 라인트레이스 결과 저장하는데 사용

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_PhysicsBody, QueryParams))
		{
			auto charCheck = Cast<APlayerOrganism>(TraceHit.GetActor());
			if (charCheck != nullptr)
			{
				focusedChar = charCheck;
			}

			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}

				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					return;
				}
			}
		}
		else
		{
			if (focusedChar != nullptr)
			{
				focusedChar = nullptr;
			}
		}
	}

	NoInteractableFound();
}

void APixelCodeCharacter::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting()) // 캐릭터가 상호작용하는 경우 호출하고 싶은 기능
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable)// 상호작용 데이터가 있으면 현재 상호작용 가능하다고 알림
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus(); // 동일하지않은 것이 나왔을때 이전 상호작용 가능 항목을 종료하는지 확인
	}

	// 상호작용 데이터를 현재 상호작용 가능 항목으로 가져오기 
	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	HUD->UpdateInteractionWidget(TargetInteractable->InteractableData); // 참조로 타겟데이터 전달

	TargetInteractable->BeginFocus();// 다음 대상 상호작용 가능 시작 호출
}

void APixelCodeCharacter::NoInteractableFound()
{
	if (IsInteracting())// 확인하고 싶은것 X
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction); // 타이머 초기화.
	}

	if (InteractionData.CurrentInteractable) // 현재 상호작용 가능 항목이 유효한 경우
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		HUD->HideInteractionWidget(); // 필요없는 위젯데이터 삭제

		InteractionData.CurrentInteractable = nullptr; // 현재 상호작용대상 nullptr
		TargetInteractable = nullptr; //대상 상호작용 가능항목 nullptr
	}
}

void APixelCodeCharacter::BeginInteract()
{
	// 작용 가능한 상태에 아무것도 변경안되었는지 확인
	PerformInteractionCheck();

	if (focusedChar != nullptr)
	{
		auto contents = focusedChar->PlayerInventory->GetInventoryContents();
		focusedChar->LootByOthers(this);
	}
	else
	{
		ServerRPC_Interact();
	}
}

// 우리가 상호작용 하고있는지 확인필요 x
void APixelCodeCharacter::EndInteract()
{

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction); // 타이머 초기화.

	if (IsValid(TargetInteractable.GetObject())) // 여전히 유효한경우
	{
		TargetInteractable->EndInteract();// 이제 대상 상호작용 가능, 대상 상호작용 종료
	}

	//lootPanelWidget->SetVisibility(ESlateVisibility::Collapsed);
	/*lootPanelWidget->RemoveFromParent();*/
}


void APixelCodeCharacter::Interact()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction); // 타이머 초기화.

	if (IsValid(TargetInteractable.GetObject())) // 여전히 유효한경우
	{
		TargetInteractable->Interact(this);// 이제 대상 상호작용 가능, 대상 상호작용 종료
	}

}


// 요한 ------------------------------------------------------------------------------------------

void APixelCodeCharacter::OnCraftingPressed()
{
	if (HUD)
	{
		//HUD->ShowOrHideCrafting();
		HUD->ToggleCreate();
	}
}

void APixelCodeCharacter::CraftItem(const FCraftItem& Item)
{
	//ReduceRecipeFromInventory(Item.CraftRecipes);

	TSubclassOf<AActor> Template = ItemStorage->GetTemplateOfItem(Item.CraftedItem);
	if (Template)
	{
		FActorSpawnParameters Params;
		Params.Owner = this;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//FVector SpawnLoc = FVector(440.f, 0.f, 0.f);

		FVector SpawnLoc = FVector(6797.037641f, -38828.846065f, 3000.503557f);
		APickup* CraftedItem = GetWorld()->SpawnActor<APickup>(Template, SpawnLoc, FRotator(0.f), Params);
		if (CraftedItem)
		{
			//UItemBase info = CraftedItem->GetItemData();
			UE_LOG(LogTemp, Warning, TEXT("Success Spawn"));

		}
	}
}

AItemStorage* APixelCodeCharacter::GetItemStorage()
{
	return ItemStorage;
}




//TArray<UItemBase> APixelCodeCharacter::GetInventory() const
//{
//	return Inventory;
//}

//int32 APixelCodeCharacter::GetSpecifictItemAmount(EItemName ItemsName)
//{
//	int32 Amount = 0;
//	for(UItemBase& Info : Inventory)
//	{
//		if(Info.ItemName == ItemsName)
//		{
//			Amount += Info.Quantity;
//		}
//	}
//	return Amount;
//}

//void APixelCodeCharacter::ReduceRecipeFromInventory(const TArray<FRecipe>& Recipes)
//{
//	for (const FRecipe& Recipe : Recipes)
//	{
//		uint8 RecipeAmount = Recipe.Amount;
//		for (const UItemBase& Item : Inventory)
//		{
//			if (Item.ItemName == Recipe.ItemType)
//			{
//				//Item.SetQuantity(Quaternion -= Recipe.Amount)
//				//Item.SetQuantity(Recipe.Amount);
//			}
//		}
//		// FItemInfo = 아이템 베이스 , itemamount 우리 이름 방식 대로 변경
//	}
//}

// 서휘-----------------------------------------------------------------------------------------------------
void APixelCodeCharacter::SetBuildMode(bool Enabled)
{
	// B키 누르면 Enabled
	bInBuildMode = Enabled;
	UE_LOG(LogTemp, Warning, TEXT("No Builder"));

	if (Builder)
	{
		// 건축자재 preview On
		Builder->SetActorHiddenInGame(!bInBuildMode);
		UE_LOG(LogTemp, Warning, TEXT("SetBuildMode"));
	}	
}




void APixelCodeCharacter::DestroyBuildingInstance()
{
	if (bInBuildMode && Builder)
	{
		//Builder->DestroyInstance(PerformLineTrace());
	}
}

void APixelCodeCharacter::OnSetBuildModePressed()
{
	SetBuildMode(!bInBuildMode);
}

void APixelCodeCharacter::OnRemoveFoliagePressed()
{
	ServerRPC_RemoveFoliage();
}

void APixelCodeCharacter::RemoveFoliage(const FHitResult& HitResult)
{
	if(HitResult.bBlockingHit)
	{
		UFoliageInstancedStaticMeshComponent* FoliageInstance = Cast< UFoliageInstancedStaticMeshComponent>(HitResult.GetComponent());
		if(FoliageInstance)
		{
			FoliageInstance->RemoveInstance(HitResult.Item);
			GetWorld()->SpawnActor<APickup>(pickupItem, HitResult.ImpactPoint, GetActorRotation());
		}
	}
	NetMulticastRPC_RemoveFoliage(HitResult);
}

void APixelCodeCharacter::ServerRPC_RemoveFoliage_Implementation()
{
	//RemoveFoliage(PerformLineTrace());
}

void APixelCodeCharacter::NetMulticastRPC_RemoveFoliage_Implementation(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		UFoliageInstancedStaticMeshComponent* FoliageInstance = Cast< UFoliageInstancedStaticMeshComponent>(HitResult.GetComponent());
		if (FoliageInstance)
		{
			FoliageInstance->RemoveInstance(HitResult.Item);
			GetWorld()->SpawnActor<APickup>(pickupItem, HitResult.ImpactPoint, GetActorRotation());
		}
	}
}

void APixelCodeCharacter::CycleBuildingMesh()
{

	if (bInBuildMode && Builder)
	{
		// ABuildingVisual ->CycleMesh 불러옴
		// CycleMesh() = 스크롤에 따라 건축자재 메시 변경해서 preview로 보이기 
		Builder->CycleMesh();
		ServerRPC_CycleBuildingMesh();
	}
}

void APixelCodeCharacter::ServerRPC_CycleBuildingMesh_Implementation()
{
	NetMulticastRPC_CycleBuildingMesh();

}

void APixelCodeCharacter::NetMulticastRPC_CycleBuildingMesh_Implementation()
{

	Builder->CycleMesh();

}


void APixelCodeCharacter::SpawnBuilding()
{
	UE_LOG(LogTemp, Warning, TEXT("21"));
	// 2차
	//SetBuildPosition(const FHitResult & HitResult);
	// 1차
	if ( bInBuildMode && Builder)
	{ 
		// ABuilding 이 숨김이 아닐 때 = 건축자재가 preview 상태일 때
		if (Builder->BuildingClass && !Builder->IsHidden())
		
			Builder->SpawnBuilding();

			ServerRPC_SpawnBuilding();
		
		// IsHidden() --> return bHidden;
		UE_LOG(LogTemp, Warning, TEXT("---------------------------------------BUILDINGVISUAL 1ST IF"));
	
		UE_LOG(LogTemp, Warning, TEXT("SPAWNBUILDING TOP"));
	
		UE_LOG(LogTemp, Warning, TEXT("SPAWNBUILDING BOTTOM"));
		

	}
}

	void APixelCodeCharacter::OnSpawnBuildingPressed()
	{
		UE_LOG(LogTemp, Warning, TEXT("22"));
		UE_LOG(LogTemp, Warning, TEXT("PRESSED SpawnBuilding START"));
		SpawnBuilding();
		UE_LOG(LogTemp, Warning, TEXT("PRESSED SpawnBuilding END"));
	}

	void APixelCodeCharacter::ServerRPC_SpawnBuilding_Implementation()
	{
		UE_LOG(LogTemp, Warning, TEXT("23"));
		UE_LOG(LogTemp, Warning, TEXT("SERVER_SPAWNBUILDING_IMPLEMENT TOP"));
		//SpawnBuilding();

		

		UE_LOG(LogTemp, Warning, TEXT("SERVER_SPAWNBUILDING_IMPLEMENT MIDDLE"));
		NetMulticastRPC_SpawnBuilding();
	}

void APixelCodeCharacter::NetMulticastRPC_SpawnBuilding_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("24"));
	UE_LOG(LogTemp, Warning, TEXT("MULTICAST_SPAWNBUILDING_IMPLEMENT TOP"));
	Builder->SpawnBuilding();
	UE_LOG(LogTemp, Warning, TEXT("MULTICAST_SPAWNBUILDING_IMPLEMENT BOTTOM"));
}

// 서휘-----------------------------------------------------------------------------------------------------끝

void APixelCodeCharacter::ServerRPC_Interact_Implementation()
{
	if (InteractionData.CurrentInteractable) // 상호작용 데이터가 현재라면 프로세스로 돌아가게
	{
		if (IsValid(TargetInteractable.GetObject())) // 여전히 유효한경우
		{
			NetMulticastRPC_Interact(TargetInteractable);
		}
	}
}

void APixelCodeCharacter::NetMulticastRPC_Interact_Implementation(const TScriptInterface<IInteractionInterface>& Interactable)
{
	// 충돌이 발생하고 다시 상호작용 가능항목 유효한지 확인 후 상호작용 가능
	Interactable->BeginInteract();

	this->Interact();
}


float APixelCodeCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return 0.0f;
}

void APixelCodeCharacter::DieFunction()
{
	auto param = GetMesh()->GetCollisionResponseToChannels();
	param.SetResponse(ECC_Visibility, ECollisionResponse::ECR_Block);

	GetMesh()->SetCollisionResponseToChannels(param);

	if (IsLocallyControlled())
	{
		auto pc = Cast<APlayerController>(Controller);

		if (pc)
		{
			DisableInput(pc);
		}
	}

	motionState = ECharacterMotionState::Die;

	Super::DieFunction();
}

void APixelCodeCharacter::CreateInventory()
{
	// Do Not Super Call
	if (PlayerInventory == nullptr)
	{
		PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	}

	PlayerInventory->SetSlotsCapacity(60); //인벤토리 슬롯 20개생성
	PlayerInventory->SetWeightCapacity(1000.0f); // 무게용량 50설정
}

void APixelCodeCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APixelCodeCharacter, InteractionData);
	DOREPLIFETIME(APixelCodeCharacter, TargetInteractable);
	DOREPLIFETIME(APixelCodeCharacter, BuildingClass);
	DOREPLIFETIME(APixelCodeCharacter, Builder);
	DOREPLIFETIME(APixelCodeCharacter, Buildings);

}

void APixelCodeCharacter::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInteractionWidget(TargetInteractable->InteractableData); // 포인터(*)확인해볼것
	}
}



void APixelCodeCharacter::ToggleMenu()
{
	HUD->ToggleMenu();
}

void APixelCodeCharacter::StatMenu()
{
	if (bIsStatVisible)
	{ 
		statWidget->DisplayStat();
		bIsStatVisible = false;
	}
	
	else
	{
		statWidget->HideStat();
		bIsStatVisible = true;
	}
}

void APixelCodeCharacter::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	// 인벤토리 null이 아니라면
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// 캐릭터 50앞방향에서 생성됨
		const FVector SpawnLocation{ GetActorLocation() + (GetActorForwardVector() * 50.0f) };

		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		// 수량제거
		const int32 RemoveQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);

		Pickup->InitializeDrop(ItemToDrop, RemoveQuantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item to drop was Some how null"));
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void APixelCodeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APixelCodeCharacter::CharacterJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APixelCodeCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APixelCodeCharacter::Look);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APixelCodeCharacter::LightAttackFunction);// 기본공격

		EnhancedInputComponent->BindAction(ToggleCombatAction, ETriggerEvent::Started, this, &APixelCodeCharacter::ToggleCombatFunction);// 무기 빼서장착

		// 인벤토리 열고닫기
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &APixelCodeCharacter::ToggleMenu);


		// 물체 상호작용
		EnhancedInputComponent->BindAction(IA_Pressed, ETriggerEvent::Started, this, &APixelCodeCharacter::BeginInteract);


		EnhancedInputComponent->BindAction(IA_Pressed, ETriggerEvent::Completed, this, &APixelCodeCharacter::EndInteract);

		EnhancedInputComponent->BindAction(IA_Stat, ETriggerEvent::Started, this, &APixelCodeCharacter::StatMenu);

		// 플레이어 구르기
		EnhancedInputComponent->BindAction(IA_RollandRun, ETriggerEvent::Canceled, this, &APixelCodeCharacter::PlayerRoll);

		// 플레이어 뛰기
		EnhancedInputComponent->BindAction(IA_RollandRun, ETriggerEvent::Ongoing, this, &APixelCodeCharacter::PlayerRun);
		EnhancedInputComponent->BindAction(IA_RollandRun, ETriggerEvent::Completed, this, &APixelCodeCharacter::PlayerRunEnd);

		// 요한 ==================
		EnhancedInputComponent->BindAction(IA_Crafting, ETriggerEvent::Started, this, &APixelCodeCharacter::OnCraftingPressed);

		// 플레이어 스킬
		EnhancedInputComponent->BindAction(IA_SkillQ, ETriggerEvent::Started, this, &APixelCodeCharacter::SkillQ);
		EnhancedInputComponent->BindAction(IA_SkillE, ETriggerEvent::Started, this, &APixelCodeCharacter::SkillE);
		EnhancedInputComponent->BindAction(IA_SkillR, ETriggerEvent::Started, this, &APixelCodeCharacter::SkillR);
		EnhancedInputComponent->BindAction(IA_SkillZ, ETriggerEvent::Started, this, &APixelCodeCharacter::SkillZ);
		EnhancedInputComponent->BindAction(IA_Skill_RightMouse, ETriggerEvent::Started, this, &APixelCodeCharacter::SkillRightMouse);

		EnhancedInputComponent->BindAction(IA_SetBuildMode, ETriggerEvent::Started, this, &APixelCodeCharacter::OnSetBuildModePressed);
		EnhancedInputComponent->BindAction(IA_RemoveFoliage, ETriggerEvent::Started, this, &APixelCodeCharacter::OnRemoveFoliagePressed);
		EnhancedInputComponent->BindAction(IA_SpawnBuilding, ETriggerEvent::Started, this, &APixelCodeCharacter::OnSpawnBuildingPressed);

		EnhancedInputComponent->BindAction(IA_Weapon, ETriggerEvent::Started, this, &APixelCodeCharacter::switchWeapon);
		EnhancedInputComponent->BindAction(IA_Weapon2, ETriggerEvent::Started, this, &APixelCodeCharacter::switchWeapon2);


	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APixelCodeCharacter::CharacterJump(const FInputActionValue& Value)
{
	if (motionState != ECharacterMotionState::Idle)
	{
		return;
	}

	Super::Jump();
}



void APixelCodeCharacter::SkillQ()
{
	Mousehit();
	
	if (false == combatComponent->bCombatEnable)
	{
		return;
	}
	if (combatComponent->bAttacking)
	{
		combatComponent->bAttackSaved = true;
	}
	else
	{
		if (bUseSkill)
		{ 
			PerformAttack(5, false);
			combatComponent->attackCount = 0;
		}
	}
}

void APixelCodeCharacter::SkillE()
{
	Mousehit();

	if (false == combatComponent->bCombatEnable)
	{
		return;
	}

	if (combatComponent->bAttacking)
	{
		combatComponent->bAttackSaved = true;
	}
	else
	{
		if (bUseSkill)
		{
			PerformAttack(6, false);
			combatComponent->attackCount = 0;
		}
	}
}

void APixelCodeCharacter::SkillR()
{
	Mousehit();

	if (false == combatComponent->bCombatEnable)
	{
		return;
	}

	if (combatComponent->bAttacking)
	{
		combatComponent->bAttackSaved = true;
	}
	else
	{
		if (bUseSkill)
		{
			PerformAttack(7, false);
			combatComponent->attackCount = 0;
		}
	}
}

void APixelCodeCharacter::SkillZ()
{
	Mousehit();
	if (false == combatComponent->bCombatEnable)
	{
		return;
	}

	if (combatComponent->bAttacking)
	{
		combatComponent->bAttackSaved = true;
	}
	else
	{
		if (bUseSkill)
		{
			PerformAttack(8, false);
			combatComponent->attackCount = 0;
		}
	}
}

void APixelCodeCharacter::SkillRightMouse()
{
	
	
	Mousehit();
	if (false == combatComponent->bCombatEnable)
	{
		return;
	}

	if (combatComponent->bAttacking)
	{
		combatComponent->bAttackSaved = true;
	}
	else
	{
		if (bUseSkill)
		{
			PerformAttack(9, false);
			combatComponent->attackCount = 0;
		}
	}
}

void APixelCodeCharacter::Mousehit()
{
	if (!bRotation)
	{
		return;
	}
		FVector cameraComponentForwardVector = FollowCamera->GetForwardVector();
		FRotator newRot = UKismetMathLibrary::MakeRotFromZX(GetActorUpVector(), cameraComponentForwardVector);

		SetActorRotation(newRot);
}

void APixelCodeCharacter::switchWeapon()
{
	FActorSpawnParameters spawnParam;
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParam.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	spawnParam.Owner = this;
	spawnParam.Instigator = this;
	
	if (equipment)
	{
		equipment->Destroy();
	}
	
	if (axe != nullptr)
	{ 
		equipment = GetWorld()->SpawnActor<ABaseWeapon>(axe, GetActorTransform(), spawnParam);
	}

	if (equipment)
	{
		equipment->OnEquipped();
	}
	
}

void APixelCodeCharacter::switchWeapon2()
{
	FActorSpawnParameters spawnParam;
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParam.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	spawnParam.Owner = this;
	spawnParam.Instigator = this;

	if (defaultWeapon != nullptr)
	{
		equipment = GetWorld()->SpawnActor<ABaseWeapon>(defaultWeapon, GetActorTransform(), spawnParam);
	}
	if (equipment)
	{
		equipment->OnEquipped();
	}
}

void APixelCodeCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APixelCodeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APixelCodeCharacter::LightAttackFunction(const FInputActionValue& Value)
{
	Mousehit();
	
	if (false == combatComponent->bCombatEnable)
	{
		return;
	}

	if (combatComponent->bAttacking)
	{
		combatComponent->bAttackSaved = true;
	}
	else
	{
		if (bUseSkill)
		{
			AttackEvent();
		}
	}
	
}



void APixelCodeCharacter::ToggleCombatFunction(const FInputActionValue& Value)
{
	auto mainWeaponPtr = combatComponent->GetMainWeapon();
	if (IsValid(mainWeaponPtr))
	{
		if (motionState == ECharacterMotionState::Idle)
		{
			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("combatComponent->bCombatEnable : %s"), combatComponent->bCombatEnable ? TEXT("TRUE") : TEXT("FALSE")));

			ServerRPC_ToggleCombat();
		}
	}
}

void APixelCodeCharacter::PlayerRoll(const FInputActionValue& Value)
{	
	if (!bRoll)
	{ 
		FVector2D LookAxisVector = Value.Get<FVector2D>();

		if (Controller != nullptr)
		{
			// add yaw and pitch input to controller
			AddControllerYawInput(LookAxisVector.X);
			AddControllerPitchInput(LookAxisVector.Y);
		}
		GetMesh()->GetAnimInstance()->Montage_Play(RollAnim);
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
		bRoll = true;
		UE_LOG(LogTemp, Warning, TEXT("Start"));
	}
}


void APixelCodeCharacter::PlayerRun(const FInputActionValue& Value)
{	
	GetCharacterMovement()->MaxWalkSpeed = 1000.f;
	UE_LOG(LogTemp,Warning,TEXT("trigreed"));
}

void APixelCodeCharacter::PlayerRunEnd(const FInputActionValue& Value)
{	
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void APixelCodeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator RotB = GetActorRotation();
	//UE_LOG(LogTemp, Log, TEXT("Rotation(Before): %f, %f, %f"), RotB.Roll, RotB.Pitch, RotB.Yaw); // Correct

	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequecy)
	{
		PerformInteractionCheck();
	}

	if (0)
	{
		PrintInfo();
	}

	// 서휘-----------------------------------------------------------------------------------------------------
	if (bInBuildMode && Builder)
	{
		///FHitResult result;
		//result = PerformLineTrace(650.0f, false);
		//UE_LOG(LogTemp,Warning,TEXT("%s"), result);
		//Builder->ServerRPC_SetBuildPosition(PerformLineTrace(650.0f, false));
		PerformLineTrace(650.0f, false);
		//UE_LOG(LogTemp, Warning, TEXT("%f,%f,%f"), result.ImpactPoint.X,result.ImpactPoint.Y,result.ImpactPoint.Z);
	}
	// 서휘-----------------------------------------------------------------------------------------------------끝

	// 지논------------------------------------------------------------------------------------------------------
	if (bRoll)
	{
		RollTime += DeltaTime;
		if (1.5f <= RollTime)
		{
			RollTime = 0;
			bRoll = false;
		}
	}
	
	if (NormallyWidgetClass)
	{
		NormallyWidget->currentStatUpdate();
	}

	if (bSkillNSQ)
	{ 
		UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_SkillQ, GetActorLocation()+GetActorForwardVector()*330, GetActorRotation());
		
		bSkillNSQ = false;
	}

	if (bSkillNSR)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_SkillR, GetActorLocation(), GetActorRotation());
		bSkillNSR = false;
	}
	// 지논------------------------------------------------------------------------------------------------------

}

void APixelCodeCharacter::PrintInfo()
{
	// localRole
	FString localRole = UEnum::GetValueAsString(GetLocalRole());

	// remoteRole
	FString remoteRole = UEnum::GetValueAsString(GetRemoteRole());

	// owner
	FString owner = GetOwner() ? GetOwner()->GetName() : "";

	// netConn
	FString netConn = GetNetConnection() ? "Valid" : "Invalid";

	FString netMode = UEnum::GetValueAsString((MyEnum)GetNetMode());

	FString hasController = Controller ? TEXT("HasController") : TEXT("NoController");

	FString strHP = FString::Printf(TEXT("%f"), stateComp->GetStatePoint(EStateType::HP));
	FString strSP = FString::Printf(TEXT("%f"), stateComp->GetStatePoint(EStateType::SP));

	FString str = FString::Printf(TEXT("localRole : %s\nremoteRole : %s\nowner : %s\nnetConn : %s\nnetMode : %s\nhasController : %s\n HP : %s\n SP : %s"), *localRole, *remoteRole, *owner, *netConn, *netMode, *hasController, *strHP, *strSP);

	FVector loc = GetActorLocation() + FVector(0, 0, 50);
	DrawDebugString(GetWorld(), loc, str, nullptr, FColor::White, 0, true);
}