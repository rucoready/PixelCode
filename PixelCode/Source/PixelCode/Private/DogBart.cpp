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
	
}

// Called every frame
void ADogBart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADogBart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* ADogBart::GetBehaviorTree() const
{
	return tree;
}

void ADogBart::ServerRPC_JumpAttack_Implementation()
{
	MulticastRPC_JumpAttack();
}

void ADogBart::MulticastRPC_JumpAttack_Implementation()
{
	PlayAnimMontage(jumpAttack);
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
	
}

