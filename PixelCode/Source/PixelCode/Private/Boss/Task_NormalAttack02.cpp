// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task_NormalAttack02.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossApernia.h"

UTask_NormalAttack02::UTask_NormalAttack02(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("NormalAttack 02");

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationFinish/AS_BossLongSwing_Montage.AS_BossLongSwing_Montage'"));
    if (montageObj.Succeeded())
    {
        swordNormalAttack02 = montageObj.Object;
    }
    bNotifyTick = true;
}

EBTNodeResult::Type UTask_NormalAttack02::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    TickTask(OwnerComp, NodeMemory, 0.0f);

    if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
    {
        APawn* ControlledPawn = bossController->GetPawn();
        if (ControlledPawn)
        {
            ACharacter* boss = Cast<ACharacter>(ControlledPawn);

            if (swordNormalAttack02 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance())
            {
                //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                boss->PlayAnimMontage(swordNormalAttack02);

            }
        }
    }
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

    return EBTNodeResult::InProgress;


}

void UTask_NormalAttack02::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // 태스크 시작 시 초기화
    if (currentTime == 0.0f)
    {
        animOnce = false;
    }

    currentTime += DeltaSeconds;

    // 1.8초가 지나면 태스크 완료
    if (currentTime >= 2.5f)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        currentTime = 0.0f; // currentTime 초기화
    }
}
