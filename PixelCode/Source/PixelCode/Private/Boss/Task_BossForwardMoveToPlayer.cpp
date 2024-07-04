// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task_BossForwardMoveToPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h" //캐릭터 임시
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "Boss/BossApernia.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"

UTask_BossForwardMoveToPlayer::UTask_BossForwardMoveToPlayer(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Forward Slash");
	
	
	bNotifyTick = true;

    static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageObj(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationFinish/AM_BossComboAttack01.AM_BossComboAttack01'"));
    if (MontageObj.Succeeded())
    {
        swordComboAttack1 = MontageObj.Object;
    }
}

EBTNodeResult::Type UTask_BossForwardMoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TickTask(OwnerComp, NodeMemory, 0.0f);
    animOnce = false;
	return EBTNodeResult::InProgress;
}

void UTask_BossForwardMoveToPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    // 태스크 시작 시 초기화
    if (currentTime == 0.0f)
    {
        animOnce = false;
    }

    currentTime += DeltaSeconds;

    if (currentTime < 2.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("OverFive1"));
        ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        if (Player)
        {
            FVector PlayerLocation = Player->GetActorLocation();
            FNavLocation RandomLocation;
            if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
            {
                if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, searchRadius, RandomLocation))
                {
                    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), RandomLocation.Location);
                    ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
                    if (bossController)
                    {
                        bossController->MoveToLocation(PlayerLocation);
                    }
                }
            }
        }

        AAIController* bossController = Cast<AAIController>(OwnerComp.GetOwner());
        if (bossController)
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ACharacter* boss = Cast<ACharacter>(ControlledPawn);

                if (swordComboAttack1 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnce)
                {
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();
                    boss->PlayAnimMontage(swordComboAttack1);
                    animOnce = true;
                }
            }
        }
    }


    if (currentTime > 3.45f &&!swingSwordNiagaraOnce)
    {
        ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
        if (bossController)
        {

            APawn* bossPawn = bossController->GetPawn();
            if (bossPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(bossPawn);

                FVector bossLocation = boss->GetActorLocation();
                FVector bossForwardVector = boss->GetActorForwardVector();

                FVector bossSmashingLocation = bossLocation + FVector(0.0f, 0.0f, 0.0f);
                FVector bossSmashingLocationUpper = bossLocation - FVector(0.0f, 0.0f, 0.0f);

                // 보스의 회전 값을 가져옵니다.
                FRotator bossRotation = boss->GetActorRotation();

                // bossRotation에서 Roll 값에 90도를 더한 새로운 회전 값을 생성합니다.
                FRotator UpperRotation = bossRotation;
                UpperRotation.Roll += 90.0f;

                UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), impactSwing, bossSmashingLocation, boss->GetActorRotation(), FVector(3.0f));
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), impactSwing2, bossSmashingLocationUpper, UpperRotation, FVector(2.0f));
                swingSwordNiagaraOnce = true;
            }
        }
    }
    // 5초가 지나면 태스크 완료
    if (currentTime >= 4.0f)
    {
        animOnce = false;
        currentTime = 0.0f; // currentTime 초기화
        swingSwordNiagaraOnce = false;
        forwardSlash = false;
        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
        BlackboardComp = OwnerComp.GetBlackboardComponent();
        if (BlackboardComp)
        {
            BlackboardComp->SetValueAsBool(forwardSlashCoolTime.SelectedKeyName, forwardSlash);
        }
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        
    }
    
}
