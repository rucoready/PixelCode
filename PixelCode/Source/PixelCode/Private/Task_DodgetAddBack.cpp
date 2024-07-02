// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_DodgetAddBack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossApernia.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

UTask_DodgetAddBack::UTask_DodgetAddBack(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Dodge And Slash");

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AM_BackAttackDodge01_Montage.AM_BackAttackDodge01_Montage'"));
    if (montageObj.Succeeded())
    {
        dodgeAndAttack01 = montageObj.Object;
    }
    bNotifyTick = true;
}

EBTNodeResult::Type UTask_DodgetAddBack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    TickTask(OwnerComp, NodeMemory, 0.0f);

    if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
    {
        APawn* ControlledPawn = bossController->GetPawn();
        if (ControlledPawn)
        {
            ACharacter* boss = Cast<ACharacter>(ControlledPawn);

            if (dodgeAndAttack01 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance())
            {
                // 애니메이션 실행
                UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();
                boss->PlayAnimMontage(dodgeAndAttack01);

                // 플레이어의 위치를 얻어오기
                APixelCodeCharacter* const player = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
                if (player)
                {
                    playerLocation = player->GetActorLocation();
                    FVector bossLocation = boss->GetActorLocation();

                    // 플레이어 방향 벡터 계산
                    FVector directionToPlayer = playerLocation - bossLocation;
                    directionToPlayer.Z = 0; // 수평 방향만 고려

                    // 보스의 현재 방향을 기준으로 오른쪽 방향 벡터 계산
                    FRotator bossRotation = boss->GetActorRotation();
                    FVector rightVector = FRotationMatrix(bossRotation).GetScaledAxis(EAxis::Y);

                    // 플레이어 방향의 반대 방향 벡터를 계산
                    FVector backwardDirection = -directionToPlayer;
                    backwardDirection.Normalize();

                    // 우측 대각선으로 이동하는 방향 벡터 계산
                    FVector diagonalBackwardDirection = (backwardDirection + rightVector).GetSafeNormal();

                    // LaunchVelocity 설정 (임의로 크기를 조정할 수 있음)
                    FVector LaunchVelocity = diagonalBackwardDirection * 8000.0f;
                    LaunchVelocity.Z = 0; // 수직 방향 제거

                    // 보스의 캐릭터를 LaunchCharacter로 발사
                    boss->LaunchCharacter(LaunchVelocity, true, true);
                    UE_LOG(LogTemp, Warning, TEXT("Launch Characters!"));

                    // 보스가 플레이어를 바라보도록 회전
                    boss->SetActorRotation(directionToPlayer.Rotation());
                }
            }
        }
    }

    return EBTNodeResult::InProgress;
}

void UTask_DodgetAddBack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // 태스크 시작 시 초기화
    if (currentTime == 0.0f)
    {
        animOnce = false;

        
    }

    // 나이아가라 파티클 스폰 (두 번째 조건)
    if (currentTime > 0.1 && !onceSpawnStingNiagara)
    {
        if (ABossApernia* boss = Cast<ABossApernia>(OwnerComp.GetAIOwner()->GetPawn()))
        {
            // 보스의 위치와 방향을 가져옴
            FVector bossLocation = boss->GetActorLocation();
            FRotator bossRotation = boss->GetActorRotation();
            FVector bossForwardVector = boss->GetActorForwardVector();
            FVector bossRightVector = boss->GetActorRightVector(); // 보스의 오른쪽 벡터를 가져옴

            // 스폰 위치를 보스의 로컬 공간에서의 오프셋으로 설정
            FVector offset = bossForwardVector * -100.0f;
            offset += bossRightVector * -300.0f; // 보스의 왼쪽으로 100만큼 이동
            offset.Y -= 200.0f;
            offset.Z += 100.0f;

            // 오프셋을 월드 공간으로 변환
            FVector spawnLocation = bossLocation + bossRotation.RotateVector(offset);

            // 파티클의 회전값을 설정 (보스의 방향을 기반으로)
            FRotator spawnRotation = bossRotation + FRotator(0.0f, -250.0f, 0.0f);

            // 나이아가라 시스템을 해당 위치와 회전으로 스폰
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), doubleSwingNA2, spawnLocation, spawnRotation + FRotator(0.0f, 180.0f, 0.0f), FVector(2.0f));

            onceSpawnStingNiagara = true;
        }
    }
    currentTime += DeltaSeconds;

    // 1.8초가 지나면 태스크 완료
    if (currentTime >= 1.2f)
    {
        onceSpawnStingNiagara = false;
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

        currentTime = 0.0f; // currentTime 초기화

    }

    
}
