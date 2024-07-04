// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task_FarJumpAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossApernia.h"
#include "Camera/CameraShakeBase.h"
#include "AIController.h" // 임시
#include "Particles/ParticleSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"

UTask_FarJumpAttack::UTask_FarJumpAttack(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Far Jump Attack");

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AS_FarJumpAttack.AS_FarJumpAttack'"));
    if (montageObj.Succeeded())
    {
        farStompAttack = montageObj.Object;
    }

    static ConstructorHelpers::FClassFinder<UCameraShakeBase> shakeObj(TEXT("/Script/Engine.CameraShakeBase'/Game/KMS_AI/Boss_Alpernia/Effects/BossJumpShake.BossJumpShake_C'"));
    if (shakeObj.Succeeded())
    {
        cameraShakeOBJ = shakeObj.Class;
    }
    bNotifyTick = true;
}

EBTNodeResult::Type UTask_FarJumpAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    TickTask(OwnerComp, NodeMemory, 0.0f);

    if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
    {
        APawn* ControlledPawn = bossController->GetPawn();
        if (ControlledPawn)
        {
            ACharacter* boss = Cast<ACharacter>(ControlledPawn);

            if (farStompAttack && boss->GetMesh() && boss->GetMesh()->GetAnimInstance())
            {
                //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                boss->PlayAnimMontage(farStompAttack);

                // LaunchCharacter를 호출
                FVector LaunchVelocity(0, 0, 1500);

                boss->LaunchCharacter(LaunchVelocity, true, true);
                UE_LOG(LogTemp, Warning, TEXT("Launch Characters!"));

                APixelCodeCharacter* const player = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
                if (player)
                {
                    //플레이어의 위치를 얻어낸다
                    playerLocation = player->GetActorLocation();
                    //보스컨트롤러를 캐스팅
                    bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
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
        }
    }


    return EBTNodeResult::InProgress;
}

void UTask_FarJumpAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // 태스크 시작 시 초기화
    if (currentTime == 0.0f)
    {
        animOnce = false;
        moveToPlayerOnce = false;
        previousPlayerLocation = FVector::ZeroVector; // 플레이어의 이전 위치를 저장할 변수 초기화
    }

    // 현재 시간을 업데이트
    currentTime += DeltaSeconds;

    if (currentTime >= 1.0f && currentTime < 1.5f)
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


    // 보스 객체를 얻음
    if (ABossApernia* boss = Cast<ABossApernia>(OwnerComp.GetAIOwner()->GetPawn()))
    {
        // 플레이어 캐릭터를 얻어옴
        APixelCodeCharacter* const player = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        if (player)
        {
            playerLocation = player->GetActorLocation();

            if (currentTime >= 1.0f && currentTime < 1.5f) // 1초 후 플레이어에게 다가가기 시작
            {
                boss->bossSwordComp->SetRelativeLocation(FVector(41.750762f, 266.377071, 22.241364));
                boss->bossSwordComp->SetRelativeRotation(FRotator(-4.150309f, -5.576854f, 180.027317f));

                FVector DirectionToPlayer = playerLocation - boss->GetActorLocation();
                DirectionToPlayer.Z = 0; // 수평 이동
                float DistanceToPlayer = DirectionToPlayer.Size();

                if (DistanceToPlayer > 270.0f) // 일정 거리 이상일 때만 이동
                {
                    DirectionToPlayer.Normalize();
                    boss->SetActorLocation(boss->GetActorLocation() + DirectionToPlayer * speedAddMovement * DeltaSeconds);
                    UE_LOG(LogTemp, Warning, TEXT("Moving to Player!"));
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Close to Player, stop moving!"));
                }
            }

            if (currentTime >= 1.4f && !animOnce) // 1.5초 후 급강하 시작
            {
                previousPlayerLocation = playerLocation; // 플레이어의 위치를 저장
                FVector LaunchVelocity(0, 0, -4000);
                boss->LaunchCharacter(LaunchVelocity, true, true);
                animOnce = true;
                UE_LOG(LogTemp, Warning, TEXT("Rapid Descent!"));
            }

            if (currentTime > 1.62f && !onceCameraShake)
            {
                APlayerController* pc = player->GetController<APlayerController>();
                if (pc != nullptr)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Trying to shake camera!"));
                    pc->ClientStartCameraShake(cameraShakeOBJ);
                    onceCameraShake = true;
                }
            }

            if (currentTime > 1.7f && !onceSpawnGroundParticle01)
            {
                FVector bossLocation = boss->GetActorLocation();
                FVector bossForwardVector = boss->GetActorForwardVector();

                FVector bossGroundLocation = bossLocation - FVector(0.0f, 0.0f, boss->GetSimpleCollisionHalfHeight());
                FVector spawnLocation = bossGroundLocation + bossForwardVector * 200.0f;

                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), groundImpactParticle, spawnLocation, FRotator::ZeroRotator, FVector(0.7f));
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundImpactNiagara, spawnLocation, FRotator::ZeroRotator, FVector(0.5f));

                onceSpawnGroundParticle01 = true;
            }
        }

        if (currentTime > 3.5f)
        {
            boss->bossSwordComp->SetRelativeLocation(FVector(29.425722f, 55.060376f, 8.3646449f));
            boss->bossSwordComp->SetRelativeRotation(FRotator(4.826905f, 1.306981f, 8.324931f));
        }
    }

    // 4.5초가 지나면 태스크 완료
    if (currentTime >= 3.5f)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        currentTime = 0.0f; // currentTime 초기화
        onceCameraShake = false;
        onceSpawnGroundParticle01 = false;
    }
}
