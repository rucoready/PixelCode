// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_GruxAttack01.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h" //캐릭터 임시
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "Boss/BossApernia.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Grux.h"
#include "GruxAIController.h"
#include "GameFramework/Character.h"

UTask_GruxAttack01::UTask_GruxAttack01(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Attack01");


	bNotifyTick = true;
}

EBTNodeResult::Type UTask_GruxAttack01::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TickTask(OwnerComp, NodeMemory, 0.0f);
    currentTime = 0.0f;

    grux = Cast<AGrux>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrux::StaticClass()));
   

    grux->attackSensing = true;

	return EBTNodeResult::InProgress;

}

void UTask_GruxAttack01::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

    // currentTime 업데이트
    currentTime += DeltaSeconds;

    // Blackboard 컴포넌트 참조를 얻기
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    // Blackboard에서 가장 가까운 플레이어 위치를 가져옵니다.
    FVector nearestPlayerLocation = BlackboardComp->GetValueAsVector(TEXT("NearPlayerLoc"));

    // 보스 컨트롤러를 캐스팅
    AGruxAIController* gruxController = Cast<AGruxAIController>(OwnerComp.GetAIOwner());
    if (gruxController)
    {
        APawn* gruxPawn = gruxController->GetPawn();
        if (gruxPawn)
        {
            // 시간이 0.0초에서 0.2초 사이인 경우
            if (currentTime <= 0.2f && !animOnce)
            {
                // 애니메이션을 처리하고 서버 RPC 호출
                grux = Cast<AGrux>(gruxPawn);
                if (grux && grux->GetMesh() && grux->GetMesh()->GetAnimInstance())
                {
                    grux->ServerRPC_Attack01();
                    animOnce = true;
                }
            }

            // 시간이 0.0초에서 0.8초 사이인 경우
            if (currentTime <= 0.8f)
            {
                // 방향 설정
                FVector direction = nearestPlayerLocation - gruxPawn->GetActorLocation();
                direction.Z = 0; // 보스가 수평으로만 회전하도록 Z축 회전 제거
                FRotator newRotation = direction.Rotation();
                gruxPawn->SetActorRotation(newRotation);
            }
        }
    }

    // 시간이 3.5초 이상인 경우
    if (currentTime >= attackDelay)
    {
        currentTime = 0.0f;
        animOnce = false;
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}

