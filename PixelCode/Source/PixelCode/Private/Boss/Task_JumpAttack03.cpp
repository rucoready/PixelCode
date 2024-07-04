// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task_JumpAttack03.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Boss/BossApernia.h"

UTask_JumpAttack03::UTask_JumpAttack03(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Jump Attack 03");

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_SlashV6_Montage.AM_SlashV6_Montage'"));
    if (montageObj.Succeeded())
    {
        jumpAttack03 = montageObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj2(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_SlashV7_Montage.AM_SlashV7_Montage'"));
    if (montageObj2.Succeeded())
    {
        jumpAttack03V2 = montageObj2.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj3(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_NormalAttack03V4_Montage.AM_NormalAttack03V4_Montage'"));
    if (montageObj3.Succeeded())
    {
        jumpAttack03V3 = montageObj3.Object;
    }
    bNotifyTick = true;
}

EBTNodeResult::Type UTask_JumpAttack03::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    TickTask(OwnerComp, NodeMemory, 0.0f);

    
    APixelCodeCharacter* const player = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (player)
    {
        //플레이어의 위치를 얻어낸다
        playerLocation = player->GetActorLocation();
        //보스컨트롤러를 캐스팅
        ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
        if (bossController)
        {
            APawn* bossPawn = bossController->GetPawn();
            if (bossPawn)
            {
                // 방향 설정
                FVector direction = playerLocation - bossPawn->GetActorLocation();
                direction.Z = 0; // 보스가 수평으로만 회전하도록 Z축 회전 제거
                FRotator newRotation = direction.Rotation();
                bossPawn->SetActorRotation(newRotation);

                if (ABossApernia* boss = Cast<ABossApernia>(OwnerComp.GetAIOwner()->GetPawn()))
                {
                    
                    boss->bossSwordComp->SetRelativeLocation(FVector(17.137708f, 57.508425f, 23.246429f));
                    boss->bossSwordComp->SetRelativeRotation(FRotator(28.852794f, 169.726741f, 183.362852f));
                }
            }
        }
    }

    return EBTNodeResult::InProgress;
}

void UTask_JumpAttack03::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // 태스크 시작 시 초기화
    if (currentTime == 0.0f)
    {
        animOnce = false;
    }
    currentTime += DeltaSeconds;

    
    if (currentTime > 0.0 && currentTime < 0.2)
    {
        APixelCodeCharacter* const player = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        if (player)
        {
            //플레이어의 위치를 얻어낸다
            playerLocation = player->GetActorLocation();
            //보스컨트롤러를 캐스팅
            ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
            if (bossController)
            {
                APawn* bossPawn = bossController->GetPawn();
                if (bossPawn)
                {

                    // 방향 설정
                    FVector direction = playerLocation - bossPawn->GetActorLocation();
                    direction.Z = 0; // 보스가 수평으로만 회전하도록 Z축 회전 제거
                    FRotator newRotation = direction.Rotation();
                    bossPawn->SetActorRotation(newRotation);
                }
            }
        }
    }
    if (currentTime > 0.9 && currentTime < 1.2)
    {
        APixelCodeCharacter* const player = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        if (player)
        {
            //플레이어의 위치를 얻어낸다
            playerLocation = player->GetActorLocation();
            //보스컨트롤러를 캐스팅
            ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
            if (bossController)
            {
                APawn* bossPawn = bossController->GetPawn();
                if (bossPawn)
                {

                    // 방향 설정
                    FVector direction = playerLocation - bossPawn->GetActorLocation();
                    direction.Z = 0; // 보스가 수평으로만 회전하도록 Z축 회전 제거
                    FRotator newRotation = direction.Rotation();
                    bossPawn->SetActorRotation(newRotation);
                }
            }
        }
    }
    if (currentTime > 0.3f)
    {
        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ACharacter* boss = Cast<ACharacter>(ControlledPawn);

                if (boss->GetMesh() && !jumpOnce)
                {
                    // LaunchCharacter를 호출하여 보스를 z축으로 30만큼 날려줌
                    FVector LaunchVelocity(0, 0, 1400);

                    //boss->LaunchCharacter(LaunchVelocity, true, true);
                    UE_LOG(LogTemp, Warning, TEXT("Launch Characters!"));
                    jumpOnce = true;

                }
            }
        }
    }

    if (currentTime > 0.0 && currentTime < 1.0)
    {


        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ACharacter* boss = Cast<ACharacter>(ControlledPawn);

                if (jumpAttack03 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnceV3)
                {
                    //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                    boss->PlayAnimMontage(jumpAttack03V3);
                    animOnceV3 = true;

                }
            }
        }
    }

    if (currentTime > 0.8 && currentTime < 1.8)
    {


        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ACharacter* boss = Cast<ACharacter>(ControlledPawn);

                if (jumpAttack03 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnce)
                {
                    //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                    boss->PlayAnimMontage(jumpAttack03);
                    animOnce = true;

                }
            }
        }
    }
    if (currentTime > 1.5 && currentTime < 2.1)
    {


        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ACharacter* boss = Cast<ACharacter>(ControlledPawn);

                if (jumpAttack03V2 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnceV2)
                {
                    //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                    boss->PlayAnimMontage(jumpAttack03V2);
                    animOnceV2 = true;

                }
            }
        }
    }
    if (currentTime > 2.1f)
    {
        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ACharacter* boss = Cast<ACharacter>(ControlledPawn);

                if (boss->GetMesh() && !downOnce)
                {
                    // LaunchCharacter를 호출하여 보스를 z축으로 30만큼 날려줌
                    FVector LaunchVelocity(0, 0, -4000);

                    //boss->LaunchCharacter(LaunchVelocity, true, true);

                    downOnce = true;

                }
            }
        }
    }

    if (currentTime > 1.7f && !niagaraOnce)
    {
        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ACharacter* boss = Cast<ACharacter>(ControlledPawn);
                // 바닥 좌표 계산
                FVector bossLocation = boss->GetActorLocation();
                FVector bossGroundLocation = bossLocation - FVector(0.0f, 0.0f, boss->GetSimpleCollisionHalfHeight());

                // 이동할 거리 (임의로 정함, 여기서는 100 단위)
                float distance = 100.0f;

                // 보스의 현재 회전 값 가져오기
                FRotator bossRotation = boss->GetActorRotation();

                // 각 방향으로 회전 값 설정
                FRotator frontRotation = bossRotation;
                FRotator backRotation = bossRotation + FRotator(0.0f, 180.0f, 0.0f);
                FRotator rightRotation = bossRotation + FRotator(0.0f, 90.0f, 0.0f);
                FRotator leftRotation = bossRotation + FRotator(0.0f, -90.0f, 0.0f);

                // 각 방향으로 이동한 좌표 계산
                FVector frontLocation = bossGroundLocation + (boss->GetActorForwardVector() * distance);
                FVector backLocation = bossGroundLocation - (boss->GetActorForwardVector() * distance);
                FVector rightLocation = bossGroundLocation + (boss->GetActorRightVector() * distance);
                FVector leftLocation = bossGroundLocation - (boss->GetActorRightVector() * distance);

                // 파티클 시스템 스폰
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), groundImpactParticleV2, frontLocation, frontRotation, FVector(1.2f));
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), groundImpactParticleV2, backLocation, backRotation, FVector(1.2f));
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), groundImpactParticleV2, rightLocation, rightRotation, FVector(1.2f));
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), groundImpactParticleV2, leftLocation, leftRotation, FVector(1.2f));

                niagaraOnce = true;

                FVector bossForwardVector = boss->GetActorForwardVector();

                // 파티클을 스폰할 위치 계산
                FVector spawnLocation = bossGroundLocation + bossForwardVector * 400.0f;

                // 파티클 시스템을 월드에 스폰
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), groundImpactParticle, spawnLocation, FRotator::ZeroRotator, FVector(1.0f, 1.0f, 1.0f));

                // 플레이어 캐릭터를 얻어옴
                APixelCodeCharacter* const player = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

                if (player)
                {
                    APlayerController* pc = player->GetController<APlayerController>();
                    if (pc != nullptr)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Trying to shake camera!"));
                        pc->ClientStartCameraShake(cameraShakeOBJ);

                    }
                }

            }
        }
    }

    if (currentTime > 3.8f)
    {

        if (ABossApernia* boss = Cast<ABossApernia>(OwnerComp.GetAIOwner()->GetPawn()))
        {

            boss->bossSwordComp->SetRelativeLocation(FVector(29.425722f, 55.060376f, 8.3646449f));
            boss->bossSwordComp->SetRelativeRotation(FRotator(4.826905f, 1.306981f, 8.324931f));
        }
    }
    


    // 1.8초가 지나면 태스크 완료
    if (currentTime >= 3.5f)
    {
        currentTime = 0.0f; // currentTime 초기화
        jumpOnce = false;
        downOnce = false;
        niagaraOnce = false;
        animOnce = false;
        animOnceV2 = false;
        animOnceV3 = false;

        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
        BlackboardComp = OwnerComp.GetBlackboardComponent();
        if (BlackboardComp)
        {
            BlackboardComp->SetValueAsBool(jumpAttack3CoolTime.SelectedKeyName, jumpAttack3);
        }
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        
    }
}