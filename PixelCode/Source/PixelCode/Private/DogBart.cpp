// Fill out your copyright notice in the Description page of Project Settings.


#include "DogBart.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Boss/BossAIController.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Components/StaticMeshComponent.h"
#include "Player/PlayerOrganism.h"
#include "BossSword.h"
#include "Player/PixelCodeCharacter.h"
#include "Components/ChildActorComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/World.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/PlayerOrganism.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Sound/SoundBase.h" 
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BossMaInUI.h"
#include "GruxAIController.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "GruxAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "DogBartAIController.h"
#include "Player/pixelPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "Boss/BossAnimInstance.h"



// Sets default values
ADogBart::ADogBart()
{
	PrimaryActorTick.bCanEverTick = true;

	//sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	//SetRootComponent(sceneComp);

	//Set DogBart Mesh
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>dogBody(TEXT("/Script/Engine.SkeletalMesh'/Game/QuadrapedCreatures/Barghest/Meshes/SK_BARGHEST.SK_BARGHEST'"));
	if (dogBody.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(dogBody.Object);
	}
	
	GetMesh()->SetRelativeLocation(FVector(0, 0, -123.7));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetupAttachment(RootComponent);


	// Component 생성 및 설정
	tailMeshSM = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("dogTailMesh"));
	tailMeshSM->SetupAttachment(GetMesh());
	tailMeshSM->SetupAttachment(GetMesh(), TEXT("BARGHEST_-Tail"));

	tailMeshSM->SetRelativeLocation(FVector(49.462542, 141.242481, 0));
	tailMeshSM->SetRelativeRotation(FRotator(720.0, 143.130102, 684.539979));


	maneMeshSM = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("maneMeshSM"));
	maneMeshSM->SetupAttachment(GetMesh());
	maneMeshSM->SetupAttachment(GetMesh(), TEXT("BARGHEST_-Spine1"));

	
	maneMeshSM->SetRelativeLocation(FVector(-13.095773, -141.529826, -2.570019));
	maneMeshSM->SetRelativeRotation(FRotator(88.111502, -48.841964, 41.910342));
	maneMeshSM->SetWorldScale3D(FVector(1.2));
	
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	
	damageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("damageBox"));
	damageBox->SetupAttachment(GetMesh(), TEXT("BARGHEST_-Head"));
	damageBox->SetRelativeLocation(FVector(50, -20, 0));
	damageBox->SetWorldScale3D(FVector(1.25,1.0,1.25));
	


	//static ConstructorHelpers::FObjectFinder<USkeletalMesh>dogMane(TEXT("/Script/Engine.SkeletalMesh'/Game/QuadrapedCreatures/Barghest/Meshes/SK_BARGHEST_MANE.SK_BARGHEST_MANE'"));
	//if (dogMane.Succeeded())
	//{
	//	GetMesh()->SetSkeletalMesh(dogMane.Object);
	//}
	//USkeletalMeshComponent* dogManeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("dogTailMesh"));
	//dogManeMesh->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void ADogBart::BeginPlay()
{
	Super::BeginPlay();
	damageBox->OnComponentBeginOverlap.AddDynamic(this, &ADogBart::OnBeginOverlapDamageCollision);

	currentHp = maxHp;
}

// Called every frame
void ADogBart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (currentHp <= 0.0f && !onceDie)
	{
		onceDie = true;
		
		ServerRPC_Die();
		
		UGameplayStatics::PlaySoundAtLocation(this, dieSound, GetActorLocation());
		


		GetWorldTimerManager().SetTimer(timerhandle_Destroy, this, &ADogBart::DestroySelf, 3.0f, false);
		


		if (ADogBartAIController* dogBartController = Cast<ADogBartAIController>(GetController()))
		{
			dogBartController->ClearFocus(EAIFocusPriority::Default);
			dogBartController->StopMovement();


			if (UBehaviorTreeComponent* BTComponent = dogBartController->FindComponentByClass<UBehaviorTreeComponent>())
			{
				BTComponent->StopTree(EBTStopMode::Safe);
			}
		}
	}

}

// Called to bind functionality to input
void ADogBart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADogBart::ApplyDamageToTarget(AActor* OtherActor, float DamageAmount)
{
	if (IsValid(OtherActor))
	{
		Pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, Pc, Player, UDamageType::StaticClass());


	}
}

void ADogBart::Redamage()
{
	reDamage = false;
}

void ADogBart::DestroySelf()
{
	Destroy();

}

void ADogBart::DogBartTakeDamage(float Damage)
{
	currentHp = currentHp - Damage;

	int32 valueWhining = FMath::RandRange(1, 2);
	{
		if (valueWhining == 1)
		{
			UGameplayStatics::PlaySoundAtLocation(this, whiningSound1, GetActorLocation());
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(this, whiningSound2, GetActorLocation());
		}
	}
}

void ADogBart::DamageCollisionActive()
{
	damageBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetWorldTimerManager().SetTimer(Timerhandle_CooltimeDamageCollisionActive, this, &ADogBart::DamageCollisionDeactive, 0.5f, false);
}

void ADogBart::DamageCollisionDeactive()
{
	damageBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADogBart::OnBeginOverlapDamageCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains("Player")&&!reDamage)
	{

		Player = Cast<APixelCodeCharacter>(OtherActor);
		if (Player)
		{
				Player->GetHit(SweepResult.ImpactPoint, false);
				UGameplayStatics::PlaySoundAtLocation(this, biteSound, GetActorLocation());
		}
		ApplyDamageToTarget(OtherActor, 5);
		reDamage = true;
		GetWorldTimerManager().SetTimer(timerhandle_CoolTimeReDamage, this, &ADogBart::Redamage, 0.8f, false);
	}
}

UBehaviorTree* ADogBart::GetBehaviorTree() const
{
	return tree;
}

void ADogBart::ServerRPC_TakeDamage_Implementation()
{
	MulticastRPC_TakeDamage();
}

void ADogBart::MulticastRPC_TakeDamage_Implementation()
{
	PlayAnimMontage(takeDamage1);
}

void ADogBart::ServerRPC_JumpAttack_Implementation()
{
	MulticastRPC_JumpAttack();
}

void ADogBart::MulticastRPC_JumpAttack_Implementation()
{
	PlayAnimMontage(jumpAttack);

	int32 value = FMath::RandRange(1, 5);
	{
		if (value == 1)
		{
			UGameplayStatics::PlaySoundAtLocation(this, barkSound1, GetActorLocation());
		}
		else if(value == 2)
		{
			UGameplayStatics::PlaySoundAtLocation(this, barkSound2, GetActorLocation());
		}
		else if (value == 3)
		{
			UGameplayStatics::PlaySoundAtLocation(this, barkSound3, GetActorLocation());
		}
		else if (value == 4)
		{
			UGameplayStatics::PlaySoundAtLocation(this, barkSound4, GetActorLocation());
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(this, barkSound5, GetActorLocation());
		}
	}
}

void ADogBart::ServerRPC_MeleeAttack_Implementation()
{
	MulticastRPC_MeleeAttack();
	
}

void ADogBart::MulticastRPC_MeleeAttack_Implementation()
{
	
	int32 value = FMath::RandRange(1, 2);
	{
		if (value == 1)
		{
			PlayAnimMontage(meleeAttack);
		}
		else
		{
			PlayAnimMontage(meleeAttack2);
		}
	}

	int32 valueBark = FMath::RandRange(1, 5);
	{
		if (valueBark == 1)
		{
			UGameplayStatics::PlaySoundAtLocation(this, barkSound1, GetActorLocation());
		}
		else if (valueBark == 2)
		{
			UGameplayStatics::PlaySoundAtLocation(this, barkSound2, GetActorLocation());
		}
		else if (valueBark == 3)
		{
			UGameplayStatics::PlaySoundAtLocation(this, barkSound3, GetActorLocation());
		}
		else if (valueBark == 4)
		{
			UGameplayStatics::PlaySoundAtLocation(this, barkSound4, GetActorLocation());
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(this, barkSound5, GetActorLocation());
		}
	}
	
}

void ADogBart::ServerRPC_Die_Implementation()
{
	MulticastRPC_Die();
}

void ADogBart::MulticastRPC_Die_Implementation()
{
	PlayAnimMontage(die);
}

void ADogBart::ServerRPC_GrowlSound_Implementation()
{
	if (!growlSoundPlayOnce)
	{
		MulticastRPC_GrowlSound();
		growlSoundPlayOnce = true;
		GetWorldTimerManager().SetTimer(timerhandle_RestoreGrowlSound, this, &ADogBart::ReSoundPlayGrowlSound, 3.0f, false);
	}
	
}

void ADogBart::MulticastRPC_GrowlSound_Implementation()
{
	int32 value = FMath::RandRange(1, 5);
	{
		if (value == 1)
		{
			UGameplayStatics::PlaySoundAtLocation(this, dogGrowl1, GetActorLocation());
		}
		else if (value == 2)
		{
			UGameplayStatics::PlaySoundAtLocation(this, dogGrowl2, GetActorLocation());
		}
		else if (value == 3)
		{
			UGameplayStatics::PlaySoundAtLocation(this, dogGrowl3, GetActorLocation());
		}
		else if (value == 4)
		{
			UGameplayStatics::PlaySoundAtLocation(this, dogGrowl4, GetActorLocation());
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(this, dogGrowl5, GetActorLocation());
		}
	}
}

void ADogBart::ReSoundPlayGrowlSound()
{
	growlSoundPlayOnce = false;
}
