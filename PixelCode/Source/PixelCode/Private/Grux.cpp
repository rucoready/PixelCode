// Fill out your copyright notice in the Description page of Project Settings.


#include "Grux.h"
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
#include "Player/pixelPlayerState.h"

// Sets default values
AGrux::AGrux()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


    //Set Character Mesh
    static ConstructorHelpers::FObjectFinder<USkeletalMesh>gruxMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/KMS_AI/Grux/GruxMolten.GruxMolten'"));
    if (gruxMesh.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(gruxMesh.Object);
    }
    GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
    GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

    boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("boxCollision"));
    
    boxCollision->SetupAttachment(GetMesh());

    // 부모 메쉬에 Attach
    boxCollision->SetupAttachment(GetMesh(), TEXT("weapon_l"));

    boxCollision->SetRelativeLocation(FVector(0, 85.0, 0));
    boxCollision->SetWorldScale3D(FVector(1.0,2.5,1.0));

    boxCollisionR = CreateDefaultSubobject<UBoxComponent>(TEXT("boxCollisionR"));

    boxCollisionR->SetupAttachment(GetMesh());

    // 부모 메쉬에 Attach
    boxCollisionR->SetupAttachment(GetMesh(), TEXT("weapon_r"));

    boxCollisionR->SetRelativeLocation(FVector(0, -94.0, 0));
    boxCollisionR->SetWorldScale3D(FVector(1.0, 2.5, 1.0));
}

// Called when the game starts or when spawned
void AGrux::BeginPlay()
{
	Super::BeginPlay();
    gruxDie = false;
    currentHp = maxHp;
    originLocation = GetActorLocation();
    float radius2D = 3000.0f;

    FVector direction2D = FVector(1.0f, 0.0f, 0.0f);

    maxLocation = originLocation + FVector(radius2D, 0.0f, 0.0f); // X축 방향으로 반지름만큼 떨어진 위치로 설정
    

    DrawDebugSphere(GetWorld(), originLocation, radius2D, 24, FColor::Green, true, -1, 0, 15.0f);

    maxLocationSize = maxLocation.Size2D();

    boxCollision->OnComponentBeginOverlap.AddDynamic(this, &AGrux::OnBeginOverlapSwordCollision);
    boxCollisionR->OnComponentBeginOverlap.AddDynamic(this, &AGrux::OnBeginOverlapSwordCollision);
}

// Called every frame
void AGrux::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    currentLocation = GetActorLocation();

    // 위치 비교: originLocation으로부터의 거리를 계산
     distance = FVector::Dist(originLocation, currentLocation);
 
     if (distance > maxLocationSize && !returnCoolTime)

     {
         RetrunOriginLoc();
         returnCoolTime = true;
         
     }
     if (currentHp <= 0 &&!dieOnce)
     {
         dieOnce = true;
         gruxDie = true;
         


         GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
         GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

         GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
         //GetMesh()->SetAnimation(gruxDie2);
         if (AGruxAIController* gruxController = Cast<AGruxAIController>(GetController()))
         {
             gruxController->ClearFocus(EAIFocusPriority::Default);
             gruxController->StopMovement();


             if (UBehaviorTreeComponent* BTComponent = gruxController->FindComponentByClass<UBehaviorTreeComponent>())
             {
                 BTComponent->StopTree(EBTStopMode::Safe);
             }
             GetWorldTimerManager().SetTimer(timerhandle_Destroy, this, &AGrux::DestroySelf, 7.0f, false);

         }
         UE_LOG(LogTemp, Warning, TEXT("GRUX DIE!"));

         //UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
         //if (animInstance)
         //{
             // BossAnimInstance 타입으로 캐스팅합니다.
         //    UGruxAnimInstance* gruxInstance = Cast<UGruxAnimInstance>(animInstance);
         //    if (gruxInstance)
         //    {
         //        gruxInstance = NULL;
         //    }
         //}

         
         ServerRPC_GruxDie();
         

         


     }



}

// Called to bind functionality to input
void AGrux::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* AGrux::GetBehaviorTree() const
{
    return tree;
}

void AGrux::CoolTimeSetting()
{
    returnCoolTime = false;
}

void AGrux::GruxTakeDamage(float Damage)
{
    currentHp = currentHp - Damage;

    
    float PulseStrength = 3000.0f;  // 펄스의 세기 
    FVector LaunchDirection = -GetActorForwardVector(); // 뒷 방향으로 설정
    LaunchCharacter(LaunchDirection * PulseStrength, true, true);

    

    ServerRPC_GruxCameraShake();

}

void AGrux::DestroySelf()
{
    Destroy();
}

void AGrux::RepocessBehaviorTreeRe()
{
    if (AGruxAIController* gruxController = Cast<AGruxAIController>(GetController()))
    {
        if (tree)
        {
            gruxController->RunBehaviorTree(tree);
        }

    }
    GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void AGrux::SwordCollisionActive()
{
    boxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    boxCollisionR->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AGrux::SwordCollisionDeactive()
{
    boxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    boxCollisionR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGrux::ResetOverlapCoolTime()
{
    cooltimeReset = false;
}

void AGrux::OnBeginOverlapSwordCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->GetName().Contains("Player")&&!cooltimeReset)
    {
        ApplyDamageToTarget(OtherActor, 7);
        cooltimeReset = true;
        GetWorldTimerManager().SetTimer(timerhandle_CoolTimeOverlap, this, &AGrux::ResetOverlapCoolTime, 0.5f, false);
    }
}

void AGrux::ApplyDamageToTarget(AActor* OtherActor, float DamageAmount)
{
    if (IsValid(OtherActor))
    {
        Pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
        UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, Pc, Player, UDamageType::StaticClass());
    }
}

void AGrux::RetrunOriginLoc()
{
    
    
    if (AGruxAIController* gruxController = Cast<AGruxAIController>(GetController()))
    {
        gruxController->ClearFocus(EAIFocusPriority::Default);
        gruxController->StopMovement();


        if (UBehaviorTreeComponent* BTComponent = gruxController->FindComponentByClass<UBehaviorTreeComponent>())
        {
            BTComponent->StopTree(EBTStopMode::Safe);
        }
        GetWorldTimerManager().SetTimer(timerhandle_RepocessBehaviorTree, this, &AGrux::RepocessBehaviorTreeRe, 5.0f, false);

    }
    if (AGruxAIController* gruxController = Cast<AGruxAIController>(GetController()))
    {
        gruxController->MoveToLocation(originLocation);

    }
    
    GetCharacterMovement()->MaxWalkSpeed = 900.0f;
    UE_LOG(LogTemp, Warning, TEXT("ReP Grux Tree"));

    GetWorldTimerManager().SetTimer(timerhandle_ReturnOrigin, this, &AGrux::CoolTimeSetting, 5.f, false);
    
}

void AGrux::ServerRPC_GruxCameraShake_Implementation()
{
    MulticastRPC_GruxCameraShake();
}

void AGrux::MulticastRPC_GruxCameraShake_Implementation()
{
    FVector gruxLocation = GetActorLocation();
    float radius = 200.0f;  // 200m 범위


    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

    for (AActor* actor : foundCharacters)
    {
        APixelCodeCharacter* player = Cast<APixelCodeCharacter>(actor);
        if (player)
        {
            // 플레이어의 위치 가져오기
            FVector playerLocation = player->GetActorLocation();
            // Grux와 플레이어 사이의 거리 계산
            float distanceP = FVector::Dist(gruxLocation, playerLocation);

            // 거리가 200m 이하일 경우 카메라 흔들기 실행
            if (distanceP <= radius)
            {
                APlayerController* pc = player->GetController<APlayerController>();
                if (pc != nullptr)
                {
                    pc->ClientStartCameraShake(cameraShakeOBJ);
                }
            }
        }
    }
}

void AGrux::ServerRPC_GruxDie_Implementation()
{
    MulticastRPC_GruxDie();

	/*Player = Cast<APixelCodeCharacter>(Player->GetOwner());
	if (Player != nullptr)
	{
		PlayerState = Cast<ApixelPlayerState>(Player->GetPlayerState());
		if (PlayerState != nullptr)
		{
			PlayerState->addUpEXP(30.0f);
			UE_LOG(LogTemp, Warning, TEXT("GruxDie"));
		}
	}*/
}

void AGrux::MulticastRPC_GruxDie_Implementation()
{
    int32 value2 = FMath::RandRange(1, 2);

    if (value2 == 1)
    {
        PlayAnimMontage(gruxDie1);

    }
    else
    {
        PlayAnimMontage(gruxDie2);

    }
}

void AGrux::ServerRPC_TakeDamage_Implementation()
{
    MulticastRPC_TakeDamage();
}

void AGrux::MulticastRPC_TakeDamage_Implementation()
{
    int32 value2 = FMath::RandRange(1, 3);

    if (value2 == 1)
    {
        PlayAnimMontage(gruxTakeDamage);

    }
    else if (value2 == 2)
    {
        PlayAnimMontage(gruxTakeDamage2);

    }
    else
    {
        PlayAnimMontage(gruxTakeDamage3);

    }
}

void AGrux::ServerRPC_Attack01_Implementation()
{
    MulticastRPC_Attack01();
}

void AGrux::MulticastRPC_Attack01_Implementation()
{
    int32 value = FMath::RandRange(1, 3);
    
    if (value == 1)
    {
        PlayAnimMontage(attack01);
        
    }
    else if(value == 2)
    {
        PlayAnimMontage(attack02);
        
    }
    else
    {
        PlayAnimMontage(attack03);
        
    }
        
}



