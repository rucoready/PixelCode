// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_DogBartJumpAttack.h"
#include "DogBartAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "DogBart.h"



UTask_DogBartJumpAttack::UTask_DogBartJumpAttack(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Dog Jump Attack");
    bNotifyTick = true;
    currentTime = 0.0f;
    animOnce = false;
}

EBTNodeResult::Type UTask_DogBartJumpAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    dogBart = Cast<ADogBart>(OwnerComp.GetAIOwner()->GetPawn());

    if (dogBart)
    {
        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
        if (BlackboardComp)
        {
            // 블랙보드에서 벡터 값을 가져옵니다.
            NearestPlayerLocation = BlackboardComp->GetValueAsVector(TEXT("DetectedPlayerLoc"));
            targetRotation = UKismetMathLibrary::FindLookAtRotation(dogBart->GetActorLocation(), NearestPlayerLocation);

            // 점프 공격 RPC 호출
            dogBart->ServerRPC_JumpAttack();

            // 초기화 수행
            currentTime = 0.0f;
            animOnce = true;

            return EBTNodeResult::InProgress;
        }
    }

    return EBTNodeResult::Failed;
}

void UTask_DogBartJumpAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
    currentTime += DeltaSeconds;

    if (dogBart)
    {
        // 현재 회전을 부드럽게 목표 회전으로 변경
        FRotator currentRotation = dogBart->GetActorRotation();
        FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, DeltaSeconds, 5.0f); // 5.0f는 보간 속도
        dogBart->SetActorRotation(newRotation);
    }

    // 4.0초가 지나면 태스크 완료
    if (currentTime >= 2.0f)
    {
        currentTime = 0.0f; // currentTime 초기화
        animOnce = false;
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
