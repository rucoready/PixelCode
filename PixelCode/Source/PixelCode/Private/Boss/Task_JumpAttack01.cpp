// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task_JumpAttack01.h"
#include "Boss/BossAIController.h"
#include "Boss/BossApernia.h"
#include "Animation/AnimInstance.h"
#include "Player/PixelCodeCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"


UTask_JumpAttack01::UTask_JumpAttack01(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Jump Attack 01");
    
    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationFinish/AM_BossComboAttack02_Montage.AM_BossComboAttack02_Montage'"));
    if (montageObj.Succeeded())
    {
        jumpAttack01 = montageObj.Object;
    }
    bNotifyTick = true;
}

EBTNodeResult::Type UTask_JumpAttack01::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    TickTask(OwnerComp, NodeMemory, 0.0f);
    animOnce = false;
    // 이동을 멈추기 위해 SimpleMoveToLocation 호출
    ABossAIController* BossAIController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
    if (BossAIController)
    {
        APawn* BossPawn = BossAIController->GetPawn();
        if (BossPawn)
        {
            BossAIController->StopMovement(); // 이동을 멈추도록 AI 컨트롤러에 지시
        }
    }

    return EBTNodeResult::InProgress;
    
    
}

void UTask_JumpAttack01::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    if (currentTime == 0.0f)
    {
        animOnce = false;
    }

    currentTime += DeltaSeconds;

    if (currentTime < 1.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("OverFive1"));
        APixelCodeCharacter* const player = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        if (player)
        {
            FVector playerLocation = player->GetActorLocation();
            OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), playerLocation);

            ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
            if (bossController)
            {
                APawn* bossPawn = bossController->GetPawn();
                if (bossPawn)
                {
                    FVector direction = playerLocation - bossPawn->GetActorLocation();
                    direction.Z = 0;
                    FRotator newRotation = direction.Rotation();
                    bossPawn->SetActorRotation(newRotation);
                }
            }
        }
    }

    APixelCodeCharacter* const player = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (player)
    {
        FVector playerLocation = player->GetActorLocation();
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), playerLocation);

        ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
        if (bossController)
        {

            APawn* bossPawn = bossController->GetPawn();
            if (bossPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(bossPawn);

                FVector bossLocation = boss->GetActorLocation();
                FVector bossForwardVector = boss->GetActorForwardVector();

                FVector bossSmashingLocation = bossLocation - FVector(0.0f, 0.0f, -100.0f);
                FVector bossGroundLocation = bossLocation - FVector(0.0f, 0.0f, boss->GetSimpleCollisionHalfHeight());
                FVector bossGroundLocation2 = bossLocation - FVector(0.0f, 0.0f, boss->GetSimpleCollisionHalfHeight());
                FVector bossGroundLocation3 = bossLocation - FVector(0.0f, 0.0f, boss->GetSimpleCollisionHalfHeight());

                FVector spawnLocation = bossGroundLocation + bossForwardVector * 200.0f;

                // -60도 회전 계산
                FRotator rotationMinus60 = boss->GetActorRotation();
                rotationMinus60.Yaw -= 15.0f;
                FVector directionMinus60 = rotationMinus60.Vector();
                FVector spawnLocation2 = bossGroundLocation2 + directionMinus60 * 200.0f;

                // +60도 회전 계산
                FRotator rotationPlus60 = boss->GetActorRotation();
                rotationPlus60.Yaw += 15.0f;
                FVector directionPlus60 = rotationPlus60.Vector();
                FVector spawnLocation3 = bossGroundLocation3 + directionPlus60 * 200.0f;

                // 현재 보스의 회전값을 가져옵니다.
                FRotator bossRotation = boss->GetActorRotation();

                // 현재 Pitch 축 값에 90도를 더한 회전값을 계산합니다.
                float newPitch = bossRotation.Pitch + 90.f;

                // Roll과 Yaw 축 값은 변하지 않도록 현재 값으로 설정합니다.
                float roll = bossRotation.Roll;
                float yaw = bossRotation.Yaw;

                // 새로운 회전값을 FRotator로 설정합니다.
                FRotator bossSmashingRotator(newPitch, yaw, roll);

                bossSmashingRotator.Yaw += -90.0f;
                

                

                if (jumpAttack01 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnce)
                {
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();
                    boss->PlayAnimMontage(jumpAttack01);

                    animOnce = true;
                }

                if (currentTime > 0.8f && !onceNiagara01)
                {
                    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundImpactSwing, bossSmashingLocation, bossSmashingRotator, FVector(3.0f));
                    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundImpact02Niagara, spawnLocation, boss->GetActorRotation(), FVector(1.5f));
                    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundImpact01Niagara, spawnLocation2, rotationMinus60, FVector(1.5f));
                    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundImpact03Niagara, spawnLocation3, rotationPlus60, FVector(1.5f));

                    onceNiagara01 = true;
                    
                }
                if (currentTime > 0.95f &&!onceNiagara02)
                {
                    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundImpactV2Niagara, spawnLocation, boss->GetActorRotation(), FVector(2.0f));
                    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundImpactV3Niagara, spawnLocation2, rotationMinus60, FVector(2.0f));
                    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundImpactV4Niagara, spawnLocation3, rotationPlus60, FVector(2.0f));
                    onceNiagara02 = true;
                }

            }
        }
    }

    if (currentTime >= 3.0f)
    {
        ABossAIController* BossAIController = Cast<ABossAIController>(OwnerComp.GetAIOwner());

        UE_LOG(LogTemp, Warning, TEXT("OverFive2"));
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        animOnce = false;
        currentTime = 0.0f;
        onceNiagara01 = false;
        onceNiagara02 = false;
    }
}
