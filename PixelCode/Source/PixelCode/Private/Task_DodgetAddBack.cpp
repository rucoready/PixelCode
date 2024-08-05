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

            if (dodgeAndAttack01 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance())
            {
                // 애니메이션 실행
                
                boss->ServerRPC_DodgeAndSlashAttack();

                
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
            boss->ServerRPC_SpawnNiagaraDodgeAndSlash();

            onceSpawnStingNiagara = true;
        }
    }
    currentTime += DeltaSeconds;

    // 1.8초가 지나면 태스크 완료
    if (currentTime >= 1.8f)
    {
        onceSpawnStingNiagara = false;
        currentTime = 0.0f; // currentTime 초기화
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

        

    }

    
}
