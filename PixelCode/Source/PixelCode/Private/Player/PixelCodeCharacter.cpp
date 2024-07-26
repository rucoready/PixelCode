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
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h>
#include "DataTypes.h"
#include "Player/SpawnSkillActor/SpawnSwordQSkill.h"
#include "Player/SpawnSkillActor/SpawnSwordRSkill.h"
#include "PCodeGameInstance.h"
#include "PCodePlayerController.h"
#include "Player/pixelPlayerState.h"
#include "MyGameModeBase.h"
#include "GameFramework/PlayerState.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "GameFramework/GameStateBase.h"
#include "FoliageISMComponent.h"



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
	
	// 요한 =======================================================================================================
	MaxInventorySlot = 30;
}

void APixelCodeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 10.0f;

	statWidget = Cast<UPlayerStatWidget>(CreateWidget(GetWorld(), StatWidgetClass));

	NormallyWidget = Cast<UNormallyWidget>(CreateWidget(GetWorld(), NormallyWidgetClass));

	/*APlayerController* UPc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	Pc = Cast<APCodePlayerController>(UPc);*/

	/*APlayerController* UPc = Cast<APlayerController>(GetController());
	Pc = Cast<APCodePlayerController>(UPc);*/



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

	/*if (Pc != nullptr && Pc->IsLocalPlayerController())
	{
		TArray<APlayerState*> psArray = GetWorld()->GetGameState()->PlayerArray;
		for (int i = 0; i < psArray.Num(); i++)
		{
			 APlayerState* ps = psArray[i];
			 PlayerState = Cast<ApixelPlayerState>(ps);
			 if (PlayerState != nullptr)
			 {
				 PlayerState->SetaddUpEXP(30.0f);
			 }
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("notPsArray"));
	}*/


	Pc = Cast<APCodePlayerController>(GetWorld()->GetFirstPlayerController());
	
	characterPlayerState();

	

	//if (HasAuthority())
	//{
	//	// 현재 플레이어의 PlayerState를 얻기
	//	auto temp = Pc->GetPlayerState<ApixelPlayerState>();

	//	if (temp == nullptr)
	//	{
	//		int iTemp = 0;
	//		iTemp = 1;
	//		iTemp = 2;
	//		iTemp = 3;
	//		iTemp = 4;
	//		iTemp = 5;
	//		iTemp = 6;

	//		if (pixelPlayerState == nullptr)
	//		{
	//			pixelPlayerState = Cast<ApixelPlayerState>(temp);
	//		}
	//	}

	//	ApixelPlayerState* CurrentPlayerState = Cast<ApixelPlayerState>(GetPlayerState());

	//	if (CurrentPlayerState != nullptr && pixelPlayerState == nullptr)
	//	{
	//		pixelPlayerState = CurrentPlayerState;
	//	}

	//	if (CurrentPlayerState != nullptr)
	//	{
	//		TArray<APlayerState*> psArray = GetWorld()->GetGameState()->PlayerArray;

	//		// psArray에서 CurrentPlayerState와 일치하는 PlayerState를 찾기
	//		for (int i = 0; i < psArray.Num(); i++)
	//		{
	//			ApixelPlayerState* PlayerStateInArray = Cast<ApixelPlayerState>(psArray[i]);
	//			if (PlayerStateInArray != nullptr && PlayerStateInArray == CurrentPlayerState)
	//			{
	//				pixelPlayerState = PlayerStateInArray;
	//				NormallyWidget->currentExpUpdate(pixelPlayerState->currentEXP, pixelPlayerState->totalEXP);
	//				//PlayerState->SetaddUpEXP(30.0f);
	//				// 일치하는 PlayerState를 찾았을 때의 로직
	//				// PlayerStateInArray를 이용해서 원하는 작업을 수행
	//				// 예: PlayerStateInArray->SomeFunction();

	//				PlayerStateInArray->PlayerId;
	//				UE_LOG(LogTemp, Warning, TEXT("%d"), PlayerStateInArray);
	//				FString Message = FString::Printf(TEXT("현재 플레이어 상태의 인덱스는 %d 입니다."), PlayerStateInArray);
	//				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Message);


	//				break; // 일치하는 것을 찾았으므로 루프 종료
	//			}
	//			else
	//			{
	//				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("플레이어 상태(PlayerState)를 찾을 수 없습니다."));
	//				UE_LOG(LogTemp, Warning, TEXT("플레이어 상태(PlayerState)를 찾을 수 없습니다."));
	//			}
	//		}
	//	}
	//	else
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("플레이어 상태(CurrentPlayerState)를 찾을 수 없습니다."));
	//		UE_LOG(LogTemp, Warning, TEXT("플레이어 상태(CurrentPlayerState)를 찾을 수 없습니다."));
	//	}
	//}

	//if (Pc != nullptr && Pc->IsLocalPlayerController())
	//{
	//	// 현재 플레이어의 PlayerState를 얻기
	//	auto temp = Pc->GetPlayerState<ApixelPlayerState>();

	//	if (temp == nullptr)
	//	{
	//		int iTemp = 0;
	//		iTemp = 1;
	//		iTemp = 2;
	//		iTemp = 3;
	//		iTemp = 4;
	//		iTemp = 5;
	//		iTemp = 6;

	//		if (pixelPlayerState == nullptr)
	//		{
	//			pixelPlayerState = Cast<ApixelPlayerState>(temp);
	//		}
	//	}

	//	ApixelPlayerState* CurrentPlayerState = Cast<ApixelPlayerState>(GetPlayerState());

	//	if (CurrentPlayerState != nullptr && pixelPlayerState == nullptr)
	//	{
	//		pixelPlayerState = CurrentPlayerState;
	//	}

	//	if (CurrentPlayerState != nullptr)
	//	{
	//		TArray<APlayerState*> psArray = GetWorld()->GetGameState()->PlayerArray;

	//		// psArray에서 CurrentPlayerState와 일치하는 PlayerState를 찾기
	//		for (int i = 0; i < psArray.Num(); i++)
	//		{
	//			ApixelPlayerState* PlayerStateInArray = Cast<ApixelPlayerState>(psArray[i]);
	//			if (PlayerStateInArray != nullptr && PlayerStateInArray == CurrentPlayerState)
	//			{
	//				pixelPlayerState = PlayerStateInArray;
	//				NormallyWidget->currentExpUpdate(pixelPlayerState->currentEXP, pixelPlayerState->totalEXP);
	//				//PlayerState->SetaddUpEXP(30.0f);
	//				// 일치하는 PlayerState를 찾았을 때의 로직
	//				// PlayerStateInArray를 이용해서 원하는 작업을 수행
	//				// 예: PlayerStateInArray->SomeFunction();

	//				PlayerStateInArray->PlayerId;
	//				UE_LOG(LogTemp,Warning,TEXT("%d"),PlayerStateInArray);
	//				FString Message = FString::Printf(TEXT("현재 플레이어 상태의 인덱스는 %d 입니다."), PlayerStateInArray);
	//				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Message);


	//				break; // 일치하는 것을 찾았으므로 루프 종료
	//			}
	//			else
	//			{
	//				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("플레이어 상태(PlayerState)를 찾을 수 없습니다."));
	//				UE_LOG(LogTemp, Warning, TEXT("플레이어 상태(PlayerState)를 찾을 수 없습니다."));
	//			}
	//		}
	//	}
	//	else
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("플레이어 상태(CurrentPlayerState)를 찾을 수 없습니다."));
	//		UE_LOG(LogTemp, Warning, TEXT("플레이어 상태(CurrentPlayerState)를 찾을 수 없습니다."));
	//	}
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("플레이어 상태(Pc)를 찾을 수 없습니다."));
	//	UE_LOG(LogTemp, Warning, TEXT("플레이어 상태(Pc)를 찾을 수 없습니다."));
	//}










	/*if (Pc != nullptr && Pc->IsLocalPlayerController())
	{
		//PlayerState = Cast<ApixelPlayerState>(GetPlayerState());
		TArray<APlayerState*> psArray = GetWorld()->GetGameState()->PlayerArray;
		for (int i = 0; i < psArray.Num(); i++)
		{

			if (PlayerState == psArray[i])
			{
				APlayerState* ps = psArray[i];
				PlayerState = Cast<ApixelPlayerState>(ps);
			}
		}
	}*/

	


	/*if (PlayerArray != nullptr)
	{
		int32 StateIndex = PlayerState->PlayerId;
		FString Message = FString::Printf(TEXT("현재 플레이어 상태의 인덱스는 %d 입니다."), StateIndex);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Message);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("플레이어 상태(PlayerState)를 찾을 수 없습니다."));
	}*/

	/*if (IsLocallyControlled())
	{
		InitRandomItem();
	}*/

	//PlayerInventory->HandleAddItem();



	// 서휘-----------------------------------------------------------------------------------------------------

// 	for (TActorIterator<ABuildingVisual> var(GetWorld()); var; ++var)
// 	{
// 		Builder = *var;
// 	}
// 
// 	for (TActorIterator<ABuilding> vars(GetWorld()); vars; ++vars)
// 	{
// 		Buildings = *vars;
// 	}

// 	if (!Builder)
// 	{
// 		for (TActorIterator<ABuildingVisual> var(GetWorld()); var; ++var)
// 		{
// 			Builder = *var;
// 		}
// 	}
// 	if (!Buildings)
// 	{
// 		for (TActorIterator<ABuilding> vars(GetWorld()); vars; ++vars)
// 		{
// 			Buildings = *vars;
// 		}
// 	}
	
	//GetAllChildActors()





 	if (!Builder)
 	{
 		if (BuildingClass)
 		{
 			//Builder = GetWorld()->SpawnActor<ABuildingVisual>(BuildingClass, FVector::ZeroVector, FRotator::ZeroRotator);
 		}
 	}
 	if (!Buildings)
 	{
 		if (BuildingC)
 		{
 			//Buildings = GetWorld()->SpawnActor<ABuilding>(BuildingC, FVector::ZeroVector, FRotator::ZeroRotator);
 		}
 	}

	FString sBuilder = Builder ? TEXT("Builder True") : TEXT("Builder False");
	FString sBuildings = Buildings ? TEXT("Buildings True") : TEXT("Buildings False");
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay : %s : %s"), *sBuilder, *sBuildings);

//   	if (!Builder)
//   	{
//   		if (BuildingClass)
//   		{
//   			Builder = GetWorld()->SpawnActor<ABuildingVisual>(BuildingClass, FVector::ZeroVector, FRotator::ZeroRotator);
//   		}
//   	}
//   	if (!Buildings)
//   	{
//   		if (BuildingC)
//   		{
//   			Buildings = GetWorld()->SpawnActor<ABuilding>(BuildingC, FVector::ZeroVector, FRotator::ZeroRotator);
//   		}
//   	}
// 
// 	FString sBuilder = Builder ? TEXT("Builder True") : TEXT("Builder False");
// 	FString sBuildings = Buildings ? TEXT("Buildings True") : TEXT("Buildings False");
// 	UE_LOG(LogTemp, Warning, TEXT("BeginPlay : %s : %s"), *sBuilder, *sBuildings);
	// 서휘-----------------------------------------------------------------------------------------------------끝

	//if (false == HasAuthority())
	//{
	//	InitMainUI();
	//}
	PlayerStartWidget();
	
	

	// 요한----------------------------------------------------------------------

	ItemStorage = GetWorld()->SpawnActor<AItemStorage>(ItemStorageTemplate, FVector::ZeroVector, FRotator::ZeroRotator);

	GameInst = Cast<UPCodeGameInstance>(UGameplayStatics::GetGameInstance(this));
	if(GameInst)
	{
		OwningInventoryntory = GameInst->LoadInventory();
	}

}


void APixelCodeCharacter::characterPlayerState()
{
	//APlayerState* CustomPlayerState = Cast<APlayerState>(this->GetPlayerState());
	//PlayerState = Cast<ApixelPlayerState>(CustomPlayerState);

	//APlayerController* UPc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//Pc = Cast<APCodePlayerController>(UPc);

	//FString netMode = GetNetMode() == ENetMode::NM_ListenServer ? TEXT("Server") : TEXT("Client");
	//FString hasController = Controller ? TEXT("HasCont") : TEXT("NoCont");

	//UE_LOG(LogTemp, Warning, TEXT("[%s] %s - PlayerState"), *netMode, *hasController);

	//PlayerState = Cast<ApixelPlayerState>(Pc->PlayerState);

	

	//ServerRPC_PlayerState();

}

//void APixelCodeCharacter::ServerRPC_PlayerState_Implementation()
//{
//	//PlayerState = Cast<ApixelPlayerState>(GetPlayerState());
//	//PlayerState = Cast<ApixelPlayerState>(Pc->PlayerState);
//	NetMulticastRPC_PlayerState();
//}
//
//void APixelCodeCharacter::NetMulticastRPC_PlayerState_Implementation()
//{
//	
//	//AGameStateBase
//	APlayerState* ps = GetPlayerState();
//	PlayerState = Cast<ApixelPlayerState>(ps);
//
//	if (ps != nullptr)
//	{
//		int32 StateIndex = PlayerState->PlayerId;
//		FString Message = FString::Printf(TEXT("현재 플레이어 상태의 인덱스는 %d 입니다."), StateIndex);
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Message);
//	}
//	else
//	{
//		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("플레이어 상태(PlayerState)를 찾을 수 없습니다."));
//	}

	//characterPlayerState();

	//APlayerState* CustomPlayerState = Cast<APlayerState>(this->GetPlayerState());
	//PlayerState = Cast<ApixelPlayerState>(Pc->PlayerState);
	//PlayerState = state;
	//PlayerState = Cast<ApixelPlayerState>(GetPlayerState());
	//PlayerState = Cast<ApixelPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(),0));
	

	/*if (PlayerState != nullptr)
	{
		int32 StateIndex = PlayerState->PlayerId;
		FString Message = FString::Printf(TEXT("현재 플레이어 상태의 인덱스는 %d 입니다."), StateIndex);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Message);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("플레이어 상태(PlayerState)를 찾을 수 없습니다."));
	}*/
	


// 서휘-----------------------------------------------------------------------------------------------------
FHitResult APixelCodeCharacter::PerformLineTrace(float Distance , bool DrawDebug)
{
	FVector Start = GetPawnViewLocation();
	FVector End = Start + GetFollowCamera()->GetForwardVector() * Distance;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

	if (DrawDebug)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0U, 3.f);
	}
	//BuildLoc = HitResult.ImpactPoint;
	return HitResult;
}
// 서휘-----------------------------------------------------------------------------------------------------끝

void APixelCodeCharacter::ServerRPC_ToggleCombat_Implementation()
{
	motionState = ECharacterMotionState::ToggleCombat;

	NetMulticastRPC_ToggleCombat();
}

void APixelCodeCharacter::NetMulticastRPC_ToggleCombat_Implementation()
{
	combatComponent->bCombatEnable = !combatComponent->bCombatEnable;

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
	if (!bIsJump)
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
}

// 우리가 상호작용 하고있는지 확인필요 x
void APixelCodeCharacter::EndInteract()
{
	if (!bIsJump)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interaction); // 타이머 초기화.

		if (IsValid(TargetInteractable.GetObject())) // 여전히 유효한경우
		{
			TargetInteractable->EndInteract();// 이제 대상 상호작용 가능, 대상 상호작용 종료
		}

		//lootPanelWidget->SetVisibility(ESlateVisibility::Collapsed);
		/*lootPanelWidget->RemoveFromParent();*/
	}
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
	if (!bIsJump)
	{
		if (HUD)
		{
			//HUD->ShowOrHideCrafting();
			HUD->ToggleCreate();
		}
	}
}

void APixelCodeCharacter::OnCraftBulkPressed()
{
	//UE_LOG(LogTemp,Warning,Text("sdsd"));
	if (!HUD)
	{
		return;
	}
	//if(HUD->getopenedwidget() == ::EOW_Crafting)
	//{
	//	// uelog
	//}

}

void APixelCodeCharacter::CraftItem(const FCraftItem& Item)
{
	ReduceRecipeFromInventory(Item.CraftRecipes);

	TSubclassOf<AActor> Template = ItemStorage->GetTemplateOfItem(Item.CraftedItem);
	if (Template)
	{

		//UInventoryComponent* OwningInventory; // 인벤토리
		FActorSpawnParameters Params;
		Params.Owner = this;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//FVector SpawnLoc = FVector(440.f, 0.f, 0.f);

		FVector SpawnLoc = FVector(6797.037641f, -38828.846065f, 3000.503557f);
		APickup* CraftedItem = GetWorld()->SpawnActor<APickup>(Template, SpawnLoc, FRotator(0.f), Params);
		if (CraftedItem)
		{
			UItemBase* ItemQuantity = CraftedItem->GetItemData();
			Iteminfos = CraftedItem->GetItemData();

			PlayerInventory->HandleAddItem(Iteminfos);
			CraftedItem->Destroy();

			UE_LOG(LogTemp, Warning, TEXT("Success Spawn"));
		}
	}
}

void APixelCodeCharacter::DropedItem(const UItemBase* Iteminfo)
{
	if (ItemStorage)
	{
		TSubclassOf<AActor> Template = ItemStorage->GetTemplateOfItem(Iteminfo->ItemName);
		if (Template)
		{

	//		//UInventoryComponent* OwningInventory; // 인벤토리
	//		FActorSpawnParameters Params;
	//		Params.Owner = this;
	//		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//		//FVector SpawnLoc = FVector(440.f, 0.f, 0.f);

	//		FVector SpawnLoc = FVector(6797.037641f, -38828.846065f, 3000.503557f);
	//		APickup* DropItemes = GetWorld()->SpawnActor<APickup>(Template, SpawnLoc, FRotator(0.f), Params);
	//		if (DropItemes)
	//		{
	//			//DropItemes->SetItemAmount(Iteminfo->Quantity);
	//		}
	//	}
	//}
}


AItemStorage* APixelCodeCharacter::GetItemStorage()
{
	return ItemStorage;
}

TArray<UItemBase*> APixelCodeCharacter::GetInventory() const
{
	return Inventory;
}

uint32 APixelCodeCharacter::GetSpecificItemAmount(EItemName ItemName)
{
	uint32 Amount = 0;
	TArray<UItemBase*> InventoryContentSArray = PlayerInventory->GetInventoryContents(); // 인벤토리 내용 배열로 가져옴

	for (UItemBase* Item : InventoryContentSArray)
	{
		if (Item && Item->ItemName == ItemName)
		{
			Amount += Item->Quantity; // 해당 아이템의 수량 누적
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("amount play"));

	return Amount;

}




void APixelCodeCharacter::BuildItem()
{

	//AGetSpecificBuildingAmount();
	//FCraftItem& Item
	FCraftItem Info = (FCraftItem)Crafts;
	//FBuildingVisualType BuildDatas = (FBuildingVisualType)Builditem;

	TSubclassOf<AActor> Template = ItemStorage->GetTemplateOfItem(Info.CraftedItem);
	if (Template)
	{

		//UInventoryComponent* OwningInventory; // 인벤토리
		FActorSpawnParameters Params;
		Params.Owner = this;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//FVector SpawnLoc = FVector(440.f, 0.f, 0.f);

		FVector SpawnLoc = FVector(6797.037641f, -38828.846065f, 3000.503557f);
		APickup* CraftedItem = GetWorld()->SpawnActor<APickup>(Template, SpawnLoc, FRotator(0.f), Params);
		if (CraftedItem)
		{
			UItemBase* ItemQuantity = CraftedItem->GetItemData();
			Iteminfos = CraftedItem->GetItemData();

			//CraftedItem->InitializeDrop(Iteminfos, );
			//DropedItem(Iteminfos);

			// 수량제거
			const int32 RemoveQuantity = PlayerInventory->RemoveAmountOfItem(Iteminfos, 1);

			//APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);

			CraftedItem->InitializeDrop(Iteminfos, RemoveQuantity);

			UE_LOG(LogTemp, Warning, TEXT("Success Destroy"));
			CraftedItem->Destroy();
		}
	}

}

void APixelCodeCharacter::AGetSpecificBuildingAmount(EBuildType builds)
{
	//for (const FRecipe& Recipe : Recipes)
	{
		TArray<UItemBase*> InventoryContentSArray = PlayerInventory->GetInventoryContents();
		//uint8 BulidAmount = Recipe.Amount;
		uint8 Index = 0;
		TArray<uint8> RemoveedIndex;


		for (UItemBase* Item : InventoryContentSArray)
		{
			if (Item && Item->Buildtypes == builds)
			{
				if (Item->Quantity -= 1)
				{
					RemoveedIndex.Add(Index);
				}
				if (Item->Quantity == 0)
				{

					//InventoryContentSArray.RemoveAt(Index);
					//PlayerInventory->RemoveAmountOfItem(Item, 1);
					PlayerInventory->FindMatchingItem(Item);
					PlayerInventory->RemoveSingleInstanceOfItem(Item);
					//PickupItems->UpdateInteractableData();
				}
			}

			Index++;

		}
		for (int8 i = RemoveedIndex.Num() - 1; i > -1; i--)
		{
			//재고가 삭제된 인벤토리
			InventoryContentSArray.RemoveAt(RemoveedIndex[i]);

		}
	}
}



void APixelCodeCharacter::ReduceRecipeFromInventory(const TArray<FRecipe>& Recipes)
{
	for (const FRecipe& Recipe : Recipes)
	{
		TArray<UItemBase*> InventoryContentSArray = PlayerInventory->GetInventoryContents();
		uint8 RecipeAmount = Recipe.Amount;
		uint8 Index = 0;
		TArray<uint8> RemoveedIndex;


		for (UItemBase* Item : InventoryContentSArray)
		{
			if (Item && Item->ItemName == Recipe.ItemType)
			{
				if (Item->Quantity - Recipe.Amount < 0)
				{
					RecipeAmount -= Item->Quantity;
					RemoveedIndex.Add(Index);
				}
				else
				{
					Item->Quantity -= RecipeAmount;
					RecipeAmount -= RecipeAmount;
					if (Item->Quantity == 0)
					{
						InventoryContentSArray.RemoveAt(Index);
						//PlayerInventory->RemoveAmountOfItem(Item, Index);
						PlayerInventory->RemoveSingleInstanceOfItem(Item);
						//PickupItems->UpdateInteractableData();
					}
					break;
				}
			}
			Index++;

		}
		for (int8 i = RemoveedIndex.Num() - 1; i > -1; i--)
		{
			//재고가 삭제된 인벤토리
			InventoryContentSArray.RemoveAt(RemoveedIndex[i]);

		}
	}

}

void APixelCodeCharacter::AddCraftArea(ECraftArea Area)
{
	CraftAreas.Add(Area);
}

void APixelCodeCharacter::RemoveArea(ECraftArea Area)
{
	CraftAreas.Remove(Area);
}

bool APixelCodeCharacter::IsPlayerInCraftArea(ECraftArea Area)
{
	if (Area == ECraftArea::ECA_Anywhere)
	{
		return true;
	}
	return CraftAreas.Contains(Area);
}

void APixelCodeCharacter::UpdateGameInstanceInventory()
{
	if (!GameInst)
	{
		return;
	}
	GameInst->UpdateInventory(OwningInventoryntory);
}



//================================요 한 끝 ===================================================

// 서휘-----------------------------------------------------------------------------------------------------

void APixelCodeCharacter::OnSetBuildModePressed()
{
	FString sbInBuildMode = !bInBuildMode ? TEXT("bInBuildMode : true") : TEXT("bInBuildMode : false");
	UE_LOG(LogTemp, Warning, TEXT("%s"), *sbInBuildMode);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!Builder)
	{
		Builder = GetWorld()->SpawnActor<ABuildingVisual>(BuildingClass, FVector::ZeroVector, FRotator::ZeroRotator);

//  		for (TActorIterator<ABuildingVisual> var(GetWorld()); var; ++var)
//  		{
//  			Builder = *var;
//  		}
	}
	if (!Buildings)
	{
		Buildings = GetWorld()->SpawnActor<ABuilding>(BuildingC, FVector::ZeroVector, FRotator::ZeroRotator);

//  		for (TActorIterator<ABuilding> vars(GetWorld()); vars; ++vars)
//  		{
//  			Buildings = *vars;
//  		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("------------------SetBuildMode @server player"));

		bInBuildMode = !GetBuildMode();
		if (Builder)
		{
			Builder->SetActorHiddenInGame(!bInBuildMode);
			UE_LOG(LogTemp, Warning, TEXT("------------------SetBuildMode @server On"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("------------------SetBuildMode : Builder nullptr"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("------------------SetBuildMode @client player"));
		ServerRPC_SetBuildMode(!GetBuildMode());
	}
}

void APixelCodeCharacter::SetBuildMode(bool Enabled)
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!Builder)
	{
		for (TActorIterator<ABuildingVisual> var(GetWorld()); var; ++var)
		{
			Builder = *var;
		}
	}
	if (!Buildings)
	{
		for (TActorIterator<ABuilding> vars(GetWorld()); vars; ++vars)
		{
			Buildings = *vars;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bInBuildMode = Enabled;
	if (Builder)
	{
		Builder->SetActorHiddenInGame(!bInBuildMode);
		UE_LOG(LogTemp, Warning, TEXT("------------------SetBuildMode : Builder"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("------------------SetBuildMode : Builder nullptr"));
	}

}

void APixelCodeCharacter::ServerRPC_SetBuildMode_Implementation(bool mode)
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!Builder)
	{
		for (TActorIterator<ABuildingVisual> var(GetWorld()); var; ++var)
		{
			Builder = *var;
		}
	}
	if (!Buildings)
	{
		for (TActorIterator<ABuilding> vars(GetWorld()); vars; ++vars)
		{
			Buildings = *vars;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 	UE_LOG(LogTemp, Warning, TEXT("------------------SetBuildMode ServerRPC"));
   	bInBuildMode = mode;
   	if (Builder)
   	{
   		Builder->SetActorHiddenInGame(!bInBuildMode);
		UE_LOG(LogTemp, Warning, TEXT("------------------SetBuildMode ServerRPC On"));
   	}
   	else
   	{
   		UE_LOG(LogTemp, Warning, TEXT("------------------SetBuildMode : Builder nullptr"));
   	}

 	MultiRPC_SetBuildMode(mode);
// 	ClientRPC_SetBuildMode(mode);
}

void APixelCodeCharacter::MultiRPC_SetBuildMode_Implementation(bool mode)
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!Builder)
	{
		for (TActorIterator<ABuildingVisual> var(GetWorld()); var; ++var)
		{
			Builder = *var;
		}
	}
	if (!Buildings)
	{
		for (TActorIterator<ABuilding> vars(GetWorld()); vars; ++vars)
		{
			Buildings = *vars;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UE_LOG(LogTemp, Warning, TEXT("------------------SetBuildMode MultiRPC"));

	bInBuildMode = mode;
  	if (Builder)
  	{
  		Builder->SetActorHiddenInGame(!bInBuildMode);
		UE_LOG(LogTemp, Warning, TEXT("------------------SetBuildMode MultiRPC On"));

	}
  	else
  	{
  		UE_LOG(LogTemp, Warning, TEXT("------------------SetBuildMode : Builder nullptr"));
  	}
}

void APixelCodeCharacter::ClientRPC_SetBuildMode_Implementation(bool mode)
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!Builder)
	{
		for (TActorIterator<ABuildingVisual> var(GetWorld()); var; ++var)
		{
			Builder = *var;
		}
	}
	if (!Buildings)
	{
		for (TActorIterator<ABuilding> vars(GetWorld()); vars; ++vars)
		{
			Buildings = *vars;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UE_LOG(LogTemp, Warning, TEXT("------------------ClientRPC"));

	//bInBuildMode = mode;
	if (Builder)
	{
		Builder->SetActorHiddenInGame(!bInBuildMode);
		UE_LOG(LogTemp, Warning, TEXT("------------------SetBuildMode ClientRPC On"));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("------------------SetBuildMode : Builder nullptr"));
	}
}

//-----------------------------------Cycle Mesh Network

void APixelCodeCharacter::OnCycleMeshPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("------------------CycleMesh Pressed"));

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("------------------CycleMesh : authority"));
		CycleBuildingMesh();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("------------------CycleMesh : no authority"));
		ServerRPC_CycleBuildingMesh();
	}
}

void APixelCodeCharacter::CycleBuildingMesh()
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!Builder)
	{
		for (TActorIterator<ABuildingVisual> var(GetWorld()); var; ++var)
		{
			Builder = *var;
		}
	}
	if (!Buildings)
	{
		for (TActorIterator<ABuilding> vars(GetWorld()); vars; ++vars)
		{
			Buildings = *vars;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FString sMode = bInBuildMode ? TEXT("Cycle_Auth BuildMode : ON") : TEXT("Cycle_Auth BuildMode : Off");
	UE_LOG(LogTemp, Warning, TEXT("------------------ %s"), *sMode);
	if (bInBuildMode && Builder)
	{
 
		Builder->CycleMesh();
	}
}

void APixelCodeCharacter::ServerRPC_CycleBuildingMesh_Implementation()
{
	CycleBuildingMesh();
	ClientRPC_CycleBuildingMesh();
}

void APixelCodeCharacter::NetMulticastRPC_CycleBuildingMesh_Implementation(UStaticMesh* newMesh)
{
	CycleBuildingMesh();

}

void APixelCodeCharacter::ClientRPC_CycleBuildingMesh_Implementation()
{
	CycleBuildingMesh();
}

										//------------------------------------Spawn Building Network

void APixelCodeCharacter::OnSpawnBuildingPressed()
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!Builder)
	{
		for (TActorIterator<ABuildingVisual> var(GetWorld()); var; ++var)
		{
			Builder = *var;
		}
	}
	if (!Buildings)
	{
		for (TActorIterator<ABuilding> vars(GetWorld()); vars; ++vars)
		{
			Buildings = *vars;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("------------------SpawnBuilding : authority"));
		SpawnBuilding();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("------------------SpawnBuilding : no authority"));
		ServerRPC_SpawnBuilding();
	}
}

void APixelCodeCharacter::SpawnBuilding()
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!Builder)
	{
		for (TActorIterator<ABuildingVisual> var(GetWorld()); var; ++var)
		{
			Builder = *var;
		}
	}
	if (!Buildings)
	{
		for (TActorIterator<ABuilding> vars(GetWorld()); vars; ++vars)
		{
			Buildings = *vars;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FString sbInBuildMode = bInBuildMode ? TEXT("BuildMode On") : TEXT("BuildMode Off");
	FString sBuilder = Builder ? TEXT("Builder true") : TEXT("Builder false");
	UE_LOG(LogTemp, Warning, TEXT("SpawnBuilding() ; %s : %s"), *sbInBuildMode, *sBuilder);

	if (bInBuildMode && Builder)
	{
		Builder->SpawnBuilding();
	}
}

void APixelCodeCharacter::ServerRPC_SpawnBuilding_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("------------------SpawnBuilding : Server"));
	SpawnBuilding();
}

void APixelCodeCharacter::NetMulticastRPC_SpawnBuilding_Implementation(EBuildType BuildType, FTransform transf)
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!Builder)
	{
		for (TActorIterator<ABuildingVisual> var(GetWorld()); var; ++var)
		{
			Builder = *var;
		}
	}
	if (!Buildings)
	{
		for (TActorIterator<ABuilding> vars(GetWorld()); vars; ++vars)
		{
			Buildings = *vars;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (Buildings)
	{
		switch (BuildType)
		{
		case EBuildType::Base:
			Buildings->BaseInstancedMesh->AddInstance(transf, true);
			break;

		case EBuildType::Wall:
			Buildings->WallInstancedMesh->AddInstance(transf, true);
			break;

		case EBuildType::Ceiling:
			Buildings->CeilingInstancedMesh->AddInstance(transf, true);
			break;

		case EBuildType::Roof:
			Buildings->RoofInstancedMesh->AddInstance(transf, true);
			break;

		case EBuildType::Gable:
			Buildings->GableInstancedMesh->AddInstance(transf, true);
			break;

		case EBuildType::Stairs:
			Buildings->StairsInstancedMesh->AddInstance(transf, true);
			break;

		case EBuildType::Window:
			Buildings->WindowInstancedMesh->AddInstance(transf, true);
			break;

		case EBuildType::Arch:
			Buildings->ArchInstancedMesh->AddInstance(transf, true);
			break;

		case EBuildType::Floor:
			Buildings->FloorInstancedMesh->AddInstance(transf, true);
			break;

		default:
			break;
		}
	}
}

//------------------------------------Destroy Building Network

void APixelCodeCharacter::OnDestroyBuildingPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("------------------Destroy Pressed"));

	ServerRPC_DestroyBuildingInstanceV2(PerformLineTrace());

	//DestroyBuildingInstance();
}

void APixelCodeCharacter::DestroyBuildingInstance()
{
	if (bInBuildMode && Builder)
	{
		Builder->DestroyInstance(PerformLineTrace());
	}
}
/////////////////////////////////////////////////
void APixelCodeCharacter::ServerRPC_DestroyBuildingInstanceV2_Implementation(const FHitResult& HitResult)
{
		MulticastRPC_DestroyBuildingInstanceV2(HitResult);

}

void APixelCodeCharacter::MulticastRPC_DestroyBuildingInstanceV2_Implementation(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		UInstancedStaticMeshComponent* Instance = Cast< UInstancedStaticMeshComponent>(HitResult.GetComponent());

		if (Instance)
		{
			UE_LOG(LogTemp, Warning, TEXT("------------------Destroy MultiRPC"));

			Instance->DestroyComponent();
		}
	}
}
/////////////////////////////////////////////////
void APixelCodeCharacter::ServerRPC_DestroyBuildingInstance_Implementation()
{
	DestroyBuildingInstance();
}

 void APixelCodeCharacter::NetMulticastRPC_DestroyBuildingInstance_Implementation(UInstancedStaticMeshComponent* instComp, int32 instIndex)
 {
	 instComp->RemoveInstance(instIndex);
 }

 //------------------------------------Remove Foliage Network

void APixelCodeCharacter::OnRemoveFoliagePressed()
{
	//ServerRPC_RemoveFoliage();

	//SeverRPC_RemoveFoliage(PerformLineTrace());
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
	//NetMulticastRPC_RemoveFoliage(HitResult);
}

void APixelCodeCharacter::SeverRPC_RemoveFoliage_Implementation(const FHitResult& HitResult)
{
	MultiRPC_RemoveFoliage(HitResult);
}

void APixelCodeCharacter::MultiRPC_RemoveFoliage_Implementation(const FHitResult& HitResult)
{

	if (HitResult.bBlockingHit)
	{
		UFoliageInstancedStaticMeshComponent* FoliageInstance = Cast< UFoliageInstancedStaticMeshComponent>(HitResult.GetComponent());
		if (FoliageInstance)
		{
			FoliageInstance->RemoveInstance(HitResult.Item);
			GetWorld()->SpawnActor<APickup>(pickupItem, HitResult.ImpactPoint, GetActorRotation());
			//GetWorld()->SpawnActor<APickup>(pickupItem, HitResult.ImpactPoint, GetActorRotation());
		}
	}
}

void APixelCodeCharacter::OnRemoveRockPressed()
{
	SeverRPC_RemoveRock(PerformLineTrace(1000, true));
}



void APixelCodeCharacter::SeverRPC_RemoveRock_Implementation(const FHitResult& HitResult)
{
	MultiRPC_RemoveRock(HitResult);
}

void APixelCodeCharacter::MultiRPC_RemoveRock_Implementation(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		
		UFoliageISMComponent* UComp = Cast<UFoliageISMComponent>(HitResult.GetComponent());
		
		if (UComp && UComp->ComponentTags.Contains(TEXT("Rock")))
		{
			UComp->RemoveInstance(HitResult.Item);

// 			if (UComp->ComponentTags.Contains(TEXT("Rock")))
// 			{
// 				UComp->RemoveInstance(HitResult.Item);
// 			}
		}
	}
}
	

// void APixelCodeCharacter::RemoveRock(const FHitResult& HitResult)
// {
// 	if (HitResult.bBlockingHit)
// 	{
// 		UFoliageInstancedStaticMeshComponent* RockInstance = Cast<UFoliageInstancedStaticMeshComponent>(HitResult.GetComponent());
// 		if (RockInstance)
// 		{
// 			RockInstance->RemoveInstance(HitResult.Item);
// 		}
// 	}
// }
// 
// void APixelCodeCharacter::SeverRPC_RemoveRock_Implementation(const FHitResult& HitResult)
// {
// 	MultiRPC_RemoveRock(HitResult);
// }
// 
// void APixelCodeCharacter::MultiRPC_RemoveRock_Implementation(const FHitResult& HitResult)
// {
// 	if (HitResult.bBlockingHit)                                       
// 	{
// 		UFoliageInstancedStaticMeshComponent* RockInstance = Cast<UFoliageInstancedStaticMeshComponent>(HitResult.GetComponent());
// 		if (RockInstance)
// 		{
// 			RockInstance->RemoveInstance(HitResult.Item);
// 		}
// 	}
// }

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
	
	if (Interactable)
	{ 
		Interactable->BeginInteract();
	}
	this->Interact();
}

float APixelCodeCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	

	return 0.0f;
}


void APixelCodeCharacter::InitMainUI()
{
	/*FString netMode = GetNetMode() == ENetMode::NM_ListenServer ? TEXT("Server") : TEXT("Client");
	FString hasController = Controller ? TEXT("HasCont") : TEXT("NoCont");

	UE_LOG(LogTemp, Warning, TEXT("[%s] %s - InitMainUI"), *netMode, *hasController);

	if (IsLocallyControlled() && NormallyWidgetClass)
	{
		auto* pc = Cast<APCodePlayerController>(Controller);
		if (nullptr == pc->NormallyWidget)
		{
			pc->NormallyWidget = Cast<UNormallyWidget>(CreateWidget(GetWorld(), NormallyWidgetClass));
			pc->NormallyWidget->AddToViewport();
		}

		NormallyWidget = pc->NormallyWidget;
	}*/
}

void APixelCodeCharacter::ServerRPC_Die_Implementation()
{
	MultiRPC_Die();
}

void APixelCodeCharacter::MultiRPC_Die_Implementation()
{
	DieFunction();
}

void APixelCodeCharacter::DieFunction()
{
	auto param = GetMesh()->GetCollisionResponseToChannels();
	param.SetResponse(ECC_Visibility, ECollisionResponse::ECR_Block);

	GetMesh()->SetCollisionResponseToChannels(param);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// UI -> 리스폰 / 종료
	if (IsLocallyControlled())
	{
		auto pc = Cast<APCodePlayerController>(Controller);
		FollowCamera->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);
		
		if (pc)
		{
			pc->SetInputMode(FInputModeUIOnly());
			pc->SetShowMouseCursor(true);
			DisableInput(pc);
			if (NormallyWidget)
			{
				NormallyWidget->SetActiveGameOverUI(true);
			}
		}
	}

	motionState = ECharacterMotionState::Die;

	Super::DieFunction();
}

void APixelCodeCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString netMode = GetNetMode() == ENetMode::NM_ListenServer ? TEXT("Server") : TEXT("Client");
	FString hasController = Controller ? TEXT("HasCont") : TEXT("NoCont");

	UE_LOG(LogTemp, Warning, TEXT("[%s] %s - PossessedBy"), *netMode, *hasController);

	// 내가 로컬이라면
	//if (IsLocallyControlled())
	
		//InitMainUI(); 나중에 활성화?
		UE_LOG(LogTemp, Warning, TEXT("Normal2"));
	
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
	DOREPLIFETIME(APixelCodeCharacter, bInBuildMode); 
	DOREPLIFETIME(APixelCodeCharacter, RollAnim);
	DOREPLIFETIME(APixelCodeCharacter, Iteminfos);
}

void APixelCodeCharacter::PlayerStartWidget()
{
	
	

	if (IsLocallyControlled())
	{
		//pc->NormallyWidget = Cast<UNormallyWidget>(CreateWidget(GetWorld(), NormallyWidgetClass));
		//pc->NormallyWidget->AddToViewport();

		// 시작
		if (StatWidgetClass)
		{
			statWidget = Cast<UPlayerStatWidget>(CreateWidget(GetWorld(), StatWidgetClass));
			//statWidget = Cast<UPlayerStatWidget>(CreateWidget(GetWorld(), StatWidgetClass));
			//statWidget = Cast<UPlayerStatWidget>(CreateWidget(GetWorld(), StatWidgetClass));
			statWidget = statWidget;
			if (statWidget != nullptr)
			{
				statWidget->AddToViewport(1);
				statWidget->SetVisibility(ESlateVisibility::Collapsed);
				UE_LOG(LogTemp, Warning, TEXT("NormalAuth"));

				statWidget->UpdateStat(this->stateComp);
				//if (PlayerState != nullptr)
				//{
				//	//Pc->statWidget->UpdateLevel(PlayerState->Level);
				//}
			}

		}
	
		if (NormallyWidgetClass)
		{
			NormallyWidget = Cast<UNormallyWidget>(CreateWidget(GetWorld(), NormallyWidgetClass));
				//NormallyWidget = Cast<UNormallyWidget>(CreateWidget(GetWorld(), NormallyWidgetClass));
			NormallyWidget = NormallyWidget;
				//NormallyWidget = Cast<UNormallyWidget>(CreateWidget(GetWorld(), NormallyWidgetClass));
				if (NormallyWidget != nullptr)
				{
					NormallyWidget->AddToViewport(-1);
					NormallyWidget->SetVisibility(ESlateVisibility::Visible);
					UE_LOG(LogTemp, Warning, TEXT("NormalAuth"));

					NormallyWidget->firstUpdate(this->stateComp);
					NormallyWidget->currentStatUpdate(this->stateComp);	
				}
		}
		
	}
		//NormallyWidget = pc->NormallyWidget;
}

void APixelCodeCharacter::FullExp()
{
	//NormallyWidget->firstStatedate();

}


void APixelCodeCharacter::PlayerLevelUp()
{
	//ApixelPlayerState* CustomPlayerState = Cast<ApixelPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(),0));
	

	//bStatExp = true;

	//PlayerState->SetaddUpEXP(30.0f);
	//if (Pc != nullptr && Pc->IsLocalPlayerController())
	//{
	//	TArray<APlayerState*> psArray = GetWorld()->GetGameState()->PlayerArray;
	//	for (int i = 0; i < psArray.Num(); i++)
	//	{
	//		PlayerState = Cast<ApixelPlayerState>(GetPlayerState());
	//		if (PlayerState == psArray[i])
	//		{
	//			//PlayerState->SetaddUpEXP(30.0f);
	//			

	//		}
	//		//APlayerState* ps = psArray[i];
	//		
	//		{
	//			
	//			
	//		}
	//			
	//		
	//	
	//		/*if (PlayerState != nullptr)
	//		{
	//			
	//		}*/
	//	}
	//}
	//PlayerState->SetaddUpEXP(30.0f);

	/*if (PlayerState)
	{
		int32 StateIndex = PlayerState->PlayerId;
		FString Message = FString::Printf(TEXT("현재 플레이어 상태의 인덱스는 %d 입니다."), StateIndex);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Message);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("플레이어 상태(PlayerState)를 찾을 수 없습니다."));
	}*/
	


}

void APixelCodeCharacter::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInteractionWidget(TargetInteractable->InteractableData); // 포인터(*)확인해볼것
	}
}



void APixelCodeCharacter::ServerRPC_DropItem_Implementation(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	
	// 인벤토리 null이 아니라면
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		// 캐릭터 50앞방향에서 생성됨
		const FVector SpawnLocation{ GetActorLocation() + (GetActorForwardVector() * 50.0f) };

		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		// 수량제거
		const int32 RemoveQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		NetMulticastRPC_DropItem(SpawnTransform);

		Pickup->InitializeDrop(ItemToDrop, RemoveQuantity);
	}
	else
		{
			UE_LOG(LogTemp, Warning, TEXT("Item to drop was Some how null"));
		}
}



	

	
void APixelCodeCharacter::NetMulticastRPC_DropItem_Implementation(const FTransform ASpawnTransform)
{
	// 인벤토리 null이 아니라면
	

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;


	Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), ASpawnTransform, SpawnParams);

}

void APixelCodeCharacter::ToggleMenu()
{
	if (!bIsJump)
	{
		HUD->ToggleMenu();
	}
}

void APixelCodeCharacter::StatMenu()
{
	if (!bIsJump)
	{
		if (bIsStatVisible)
		{
			statWidget->DisplayStat();
			bIsStatVisible = false;
			UE_LOG(LogTemp, Warning, TEXT("StatOn"));
		}

		else
		{
			statWidget->HideStat();
			bIsStatVisible = true;
			UE_LOG(LogTemp, Warning, TEXT("StatOff"));
		}
	}
}


void APixelCodeCharacter::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
//{
//	// 인벤토리 null이 아니라면
//	if (PlayerInventory->FindMatchingItem(ItemToDrop))
//	{
//		FActorSpawnParameters SpawnParams;
//		SpawnParams.Owner = this;
//		SpawnParams.bNoFail = true;
//		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
//
//		// 캐릭터 50앞방향에서 생성됨
//		const FVector SpawnLocation{ GetActorLocation() + (GetActorForwardVector() * 50.0f) };
//
//		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);
//
//		// 수량제거
//		const int32 RemoveQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);
//
//		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);
//
//		Pickup->InitializeDrop(ItemToDrop, RemoveQuantity);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Item to drop was Some how null"));
//	}
//
//	//ServerRPC_DropItem();
}



//////////////////////////////////////////////////////////////////////////
// Input

void APixelCodeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{	
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APixelCodeCharacter::CharacterJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		if (!bIsJump)
		{ 

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
			EnhancedInputComponent->BindAction(IA_CycleMesh, ETriggerEvent::Started, this, &APixelCodeCharacter::OnCycleMeshPressed);
			//EnhancedInputComponent->BindAction(IA_DestroyBuilding, ETriggerEvent::Started, this, &APixelCodeCharacter::OnDestroyBuildingPressed);

			EnhancedInputComponent->BindAction(IA_Weapon, ETriggerEvent::Started, this, &APixelCodeCharacter::switchWeapon);
			EnhancedInputComponent->BindAction(IA_Weapon2, ETriggerEvent::Started, this, &APixelCodeCharacter::switchWeapon2);
			EnhancedInputComponent->BindAction(IA_Weapon3, ETriggerEvent::Started, this, &APixelCodeCharacter::switchWeapon3);
			EnhancedInputComponent->BindAction(IA_ExpUp, ETriggerEvent::Started, this, &APixelCodeCharacter::PlayerExpUp);
			
			
			
			
			EnhancedInputComponent->BindAction(IA_RemoveRock, ETriggerEvent::Started, this, &APixelCodeCharacter::OnRemoveRockPressed);
		}

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
	if (!bIsJump)
	{
		if (!bQskillCoolTime)
		{ 
			Mousehit();
		
			if (equipment->eWeaponType != EWeaponType::LightSword)
			{
				return;
			}

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

			bQskillCoolTime = true;

			GetWorldTimerManager().SetTimer(QSkillTimer, this, &APixelCodeCharacter::QskillTime, 1.0f, true);
		}
	}
}

void APixelCodeCharacter::QskillTime()
{
	if (CurrentQSkillCoolTime >= QSkillCoolTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("Timer Function Qoff"));
		// Stop the timer
		bQskillCoolTime = false;
		CurrentQSkillCoolTime = 0;
		GetWorldTimerManager().ClearTimer(QSkillTimer);
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("%f"), GetWorld()->GetTimerManager().GetTimerElapsed(QSkillTimer)); 환록형이 알려준 코드 나중에 써보기, 1초마다 재주는 기능
	CurrentQSkillCoolTime++;
}


void APixelCodeCharacter::SkillE()
{
	if (!bIsJump)
	{
		if (!bEskillCoolTime)
		{
			Mousehit();

			if (equipment->eWeaponType != EWeaponType::LightSword)
			{
				return;
			}

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

			bEskillCoolTime = true;

			GetWorldTimerManager().SetTimer(ESkillTimer, this, &APixelCodeCharacter::EskillTime, 1.0f, true);
		}
	}
}

void APixelCodeCharacter::EskillTime()
{
	if (CurrentESkillCoolTime >= ESkillCoolTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("Timer Function Eoff"));
		// Stop the timer
		bEskillCoolTime = false;
		CurrentESkillCoolTime = 0;
		GetWorldTimerManager().ClearTimer(ESkillTimer);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Timer Function EON"));
	CurrentESkillCoolTime++;
}

void APixelCodeCharacter::SkillR()
{
	if (!bIsJump)
	{
		if (!bRskillCoolTime)
		{
			Mousehit();


			if (equipment->eWeaponType != EWeaponType::LightSword)
			{
				return;
			}

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

			bRskillCoolTime = true;

			GetWorldTimerManager().SetTimer(RSkillTimer, this, &APixelCodeCharacter::RskillTime, 1.0f, true);
		}
	}
}

void APixelCodeCharacter::RskillTime()
{
	if (CurrentRSkillCoolTime >= RSkillCoolTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("Timer Function Roff"));
		// Stop the timer
		bRskillCoolTime = false;
		CurrentRSkillCoolTime = 0;
		GetWorldTimerManager().ClearTimer(RSkillTimer);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Timer Function RON"));
	CurrentRSkillCoolTime++;
}

void APixelCodeCharacter::SkillZ()
{
	if (!bIsJump)
	{
		if (!bZskillCoolTime)
		{
			Mousehit();

			if (equipment->eWeaponType != EWeaponType::LightSword)
			{
				return;
			}

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
			bZskillCoolTime = true;

			GetWorldTimerManager().SetTimer(ZSkillTimer, this, &APixelCodeCharacter::ZskillTime, 1.0f, true);
		}
	}
}

void APixelCodeCharacter::ZskillTime()
{
	if (CurrentZSkillCoolTime >= ZSkillCoolTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("Timer Function Zoff"));
		// Stop the timer
		bZskillCoolTime = false;
		CurrentZSkillCoolTime = 0;
		GetWorldTimerManager().ClearTimer(ZSkillTimer);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Timer Function ZON"));
	CurrentZSkillCoolTime++;
}


void APixelCodeCharacter::SkillRightMouse()
{
	if (!bIsJump)
	{
		Mousehit();
		if (false == combatComponent->bCombatEnable)
		{
			return;
		}

		if (equipment->eWeaponType != EWeaponType::LightSword)
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
}




void APixelCodeCharacter::SeverRPC_QSkillSpawn_Implementation()
{
	MultiRPC_QSkillSpawn();
}

void APixelCodeCharacter::MultiRPC_QSkillSpawn_Implementation()
{
	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<ASpawnSwordQSkill>(QSkillSpawn, GetActorLocation(), GetActorRotation(), SpawnParams);
}

void APixelCodeCharacter::SeverRPC_RSkillSpawn_Implementation()
{
	MultiRPC_RSkillSpawn();
}

void APixelCodeCharacter::MultiRPC_RSkillSpawn_Implementation()
{
	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<ASpawnSwordRSkill>(RSkillSpawn, GetActorLocation(), GetActorRotation(), SpawnParams);
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
	if (!bIsJump)
	{
		FActorSpawnParameters spawnParam;
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		spawnParam.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
		spawnParam.Owner = this;
		spawnParam.Instigator = this;


		if (axe != nullptr)
		{
			equipment = GetWorld()->SpawnActor<ABaseWeapon>(axe, GetActorTransform(), spawnParam);
		}

		if (equipment)
		{
			equipment->OnEquipped();
		}

		combatComponent->bCombatEnable = true;
	}
}

void APixelCodeCharacter::switchWeapon2()
{
	if (!bIsJump)
	{
		combatComponent->bCombatEnable = false;

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
}

void APixelCodeCharacter::switchWeapon3()
{
	if (!bIsJump)
	{
		FActorSpawnParameters spawnParam;
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		spawnParam.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
		spawnParam.Owner = this;
		spawnParam.Instigator = this;


		if (Pick != nullptr)
		{
			equipment = GetWorld()->SpawnActor<ABaseWeapon>(Pick, GetActorTransform(), spawnParam);
		}

		if (equipment)
		{
			equipment->OnEquipped();
		}

		combatComponent->bCombatEnable = true;
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
	if (!bIsJump)
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
			if (equipment->eWeaponType != EWeaponType::GreatSword)
			{
				AttackEvent();
			}
			else if (equipment->eWeaponType != EWeaponType::Pick)
			{
				AttackEvent();
			}
			else if (bUseSkill)
			{
				AttackEvent();
			}
			/*if (axe != nullptr)
			{
				AttackEvent();
			}*/
		}
	}
}



void APixelCodeCharacter::ToggleCombatFunction(const FInputActionValue& Value)
{
	if (!bIsJump)
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
}

void APixelCodeCharacter::PlayerRoll(const FInputActionValue& Value)
{	
	if (!bIsJump)
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
			ServerRPC_PlayerRoll();
		}
	}
}

void APixelCodeCharacter::ServerRPC_PlayerRoll_Implementation()
{
	FTimerHandle handle;
	
	NetMulticastRPC_PlayerRoll();

	GetWorldTimerManager().SetTimer(handle, [&]() {
		float PurseStrength = 3000.0f;
		FVector CharacterForwardVector = GetActorForwardVector();
		LaunchCharacter(CharacterForwardVector * PurseStrength, true, true);
		}, 0.2f, false);

}

void APixelCodeCharacter::NetMulticastRPC_PlayerRoll_Implementation()
{
	GetMesh()->GetAnimInstance()->Montage_Play(RollAnim);
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	bRoll = true;
	UE_LOG(LogTemp, Warning, TEXT("Start"));

}

void APixelCodeCharacter::PlayerRun(const FInputActionValue& Value)
{	
	if (!bIsJump)
	{
		ServerRPC_PlayerRun();
	}
}

void APixelCodeCharacter::ServerRPC_PlayerRun_Implementation()
{
	FTimerHandle handle;

	NetMulticastRPC_PlayerRun();
}

void APixelCodeCharacter::NetMulticastRPC_PlayerRun_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.f;
	//UE_LOG(LogTemp, Warning, TEXT("trigreed"));
}

void APixelCodeCharacter::PlayerRunEnd(const FInputActionValue& Value)
{	
	if (!bIsJump)
	{
		ServerRPC_PlayerRunEnd();
	}
}

void APixelCodeCharacter::ServerRPC_PlayerRunEnd_Implementation()
{
	NetMulticastRPC_PlayerRunEnd();
}

void APixelCodeCharacter::NetMulticastRPC_PlayerRunEnd_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void APixelCodeCharacter::PlayerExpUp(const FInputActionValue& Value)
{
	PlayerLevelUp();
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
		Builder->SetBuildPosition(PerformLineTrace(2000.0f, true));
	}
	// 서휘-----------------------------------------------------------------------------------------------------끝

	if (bFarmFoliage)
	{
		SeverRPC_RemoveFoliage(PerformLineTrace());
		bFarmFoliage = false;
	}


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
	

	if (bSkillNSQ)
	{ 
		SeverRPC_QSkillSpawn();

		bSkillNSQ = false;
	}

	if (bSkillNSR)
	{
		SeverRPC_RSkillSpawn();
		
		/*FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<ASpawnSwordRSkill>(RSkillSpawn, GetActorLocation(), GetActorRotation(), SpawnParams);*/

		bSkillNSR = false;
	}

	if (NormallyWidget != nullptr)
	{
		NormallyWidget->currentStatUpdate(this->stateComp);
	}
	if (NormallyWidget != nullptr && bStatExp)
	{
		NormallyWidget->currentExpUpdate(pixelPlayerState->currentEXP, pixelPlayerState->totalEXP);
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