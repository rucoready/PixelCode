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

    
    bNotifyTick = true;
}

EBTNodeResult::Type UTask_FarJumpAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    TickTask(OwnerComp, NodeMemory, 0.0f);

    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

    int32 randomIndex = FMath::RandRange(0, foundCharacters.Num() - 1);
    player = Cast<APixelCodeCharacter>(foundCharacters[randomIndex]);



    if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
    {
        bossController->StopMovement();
        APawn* ControlledPawn = bossController->GetPawn();
        if (ControlledPawn)
        {
            ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);

            if (farStompAttack && boss->GetMesh() && boss->GetMesh()->GetAnimInstance())
            {
                //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                boss->ServerRPC_FarJumpAttack();

                // LaunchCharacter를 호출
                FVector LaunchVelocity(0, 0, 1500);

                boss->LaunchCharacter(LaunchVelocity, true, true);
                

               
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
        
        if (player)
        {
            playerLocation = player->GetActorLocation();

            if (currentTime >= 1.0f && currentTime < 1.5f) // 1초 후 플레이어에게 다가가기 시작
            {
                boss->ServerRPC_FarJumpAttackSwordPositionSet();

                FVector DirectionToPlayer = playerLocation - boss->GetActorLocation();
                DirectionToPlayer.Z = 0; // 수평 이동
                float DistanceToPlayer = DirectionToPlayer.Size();

                if (DistanceToPlayer > 270.0f) // 일정 거리 이상일 때만 이동
                {
                    DirectionToPlayer.Normalize();
                    boss->SetActorLocation(boss->GetActorLocation() + DirectionToPlayer * speedAddMovement * DeltaSeconds);
                    
                }
                else
                {
                    
                }
            }

            if (currentTime >= 1.4f && !animOnce) // 1.5초 후 급강하 시작
            {
                previousPlayerLocation = playerLocation; // 플레이어의 위치를 저장
                FVector LaunchVelocity(0, 0, -4000);
                boss->LaunchCharacter(LaunchVelocity, true, true);
                animOnce = true;
                
            }

            if (currentTime > 1.62f && !onceCameraShake)
            {
                APlayerController* pc = player->GetController<APlayerController>();
                if (pc != nullptr)
                {
                    
                    pc->ClientStartCameraShake(cameraShakeOBJ);
                    boss->ServerRPC_FarJumpAttackCameraShake();
                    boss->ServerRPC_SpawnBoundCollision2();
                    onceCameraShake = true;
                }
            }

            if (currentTime > 1.7f && !onceSpawnGroundParticle01)
            {
                boss->ServerRPC_SpawnNiagaraFarJumpAttack();

                onceSpawnGroundParticle01 = true;
            }
        }

        if (currentTime > 3.5f)
        {
            boss->ServerRPC_FarJumpAttackSwordPositionReSet();
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
