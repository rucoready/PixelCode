// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_CounterPrecursor.h"
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

UTask_CounterPrecursor::UTask_CounterPrecursor(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Counter Precursor");

	bNotifyTick = true;
}

EBTNodeResult::Type UTask_CounterPrecursor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TickTask(OwnerComp, NodeMemory, 0.0f);
	animOnce = false;

    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

    int32 randomIndex = FMath::RandRange(0, foundCharacters.Num() - 1);
    player = Cast<APixelCodeCharacter>(foundCharacters[randomIndex]);

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

void UTask_CounterPrecursor::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
    currentTime += DeltaSeconds;

    // 애니메이션 실행
    if (currentTime < 1.0f && !animOnce)
    {
        AAIController* bossController = Cast<AAIController>(OwnerComp.GetOwner());
        if (bossController)
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);

                if (boss->GetMesh() && boss->GetMesh()->GetAnimInstance())
                {
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();
                    boss->ServerRPC_CounterPrecursor();
                    animOnce = true;
                }
            }
        }
    }

    // 3초가 지나면 태스크 완료
    if (currentTime >= 2.0f)
    {
        animOnce = false;
        currentTime = 0.0f; // currentTime 초기화
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
