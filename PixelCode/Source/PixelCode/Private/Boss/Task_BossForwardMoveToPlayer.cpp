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
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
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
    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

    int32 randomIndex = FMath::RandRange(0, foundCharacters.Num() - 1);
    player = Cast<APixelCodeCharacter>(foundCharacters[randomIndex]);
    return EBTNodeResult::InProgress;
}

void UTask_BossForwardMoveToPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    CachedOwnerComp = &OwnerComp;
    CachedNodeMemory = NodeMemory;
    CachedDeltaSeconds = DeltaSeconds;

    ServerRPC_PlayComboAttack();

}

void UTask_BossForwardMoveToPlayer::TaskDoIt(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // 태스크 시작 시 초기화
    if (currentTime == 0.0f)
    {
        animOnce = false;
    }

    currentTime += DeltaSeconds;

 

    if (currentTime < 2.0f)
    {
        if (player)
        {
            FVector playerLocation = player->GetActorLocation();
            FNavLocation randomLocation;
            if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
            {
                if (NavSys->GetRandomPointInNavigableRadius(playerLocation, searchRadius, randomLocation))
                {
                    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), randomLocation.Location);
                    ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
                    if (bossController)
                    {
                        bossController->StopMovement();
                        bossController->MoveToLocation(playerLocation);
                        excuteOnceFindPlayer = true;
                    }
                }
            }
        }
        ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
        if (bossController)
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);

                if (swordComboAttack1 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnce)
                {
                    boss->ServerRPC_ForwardSlashAttack();
                    animOnce = true;
                }
            }
        }
    }
    

    if (currentTime > 3.45f && !swingSwordNiagaraOnce)
    {
        ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
        if (bossController)
        {
            APawn* bossPawn = bossController->GetPawn();
            if (bossPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(bossPawn);

                boss->ServerRPC_SpawnNiagaraForwardSlash();
                swingSwordNiagaraOnce = true;
                
            }
        }
    }

    // 5초가 지나면 태스크 완료
    if (currentTime >= 4.0f)
    {
        excuteOnceFindPlayer = false;
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

void UTask_BossForwardMoveToPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UTask_BossForwardMoveToPlayer, swordComboAttack1);
    DOREPLIFETIME(UTask_BossForwardMoveToPlayer, impactSwing);
    DOREPLIFETIME(UTask_BossForwardMoveToPlayer, impactSwing2);
    DOREPLIFETIME(UTask_BossForwardMoveToPlayer, excuteOnceFindPlayer);
    DOREPLIFETIME(UTask_BossForwardMoveToPlayer, animOnce);
    DOREPLIFETIME(UTask_BossForwardMoveToPlayer, swingSwordNiagaraOnce);
    DOREPLIFETIME(UTask_BossForwardMoveToPlayer, forwardSlash);
    DOREPLIFETIME(UTask_BossForwardMoveToPlayer, currentTime);
}

void UTask_BossForwardMoveToPlayer::ServerRPC_PlayComboAttack_Implementation()
{
    MulticastRPC_PlayNiagaraEffects();


}

void UTask_BossForwardMoveToPlayer::MulticastRPC_PlayNiagaraEffects_Implementation()
{
    // 멤버 변수를 이용하여 F1 호출
    TaskDoIt(*CachedOwnerComp, CachedNodeMemory, CachedDeltaSeconds);
}

