#include "Task_AttackWait.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Boss/BossApernia.h"

UTask_AttackWait::UTask_AttackWait(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Attack Wait");
}

EBTNodeResult::Type UTask_AttackWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 상태 초기화
    currentTime = 0.0f;

    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

    int32 randomIndex = FMath::RandRange(0, foundCharacters.Num() - 1);
    player = Cast<APixelCodeCharacter>(foundCharacters[randomIndex]);

    if (player)
    {
        playerLocation = player->GetActorLocation();

        ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
        if (bossController)
        {
            APawn* bossPawn = bossController->GetPawn();
            if (bossPawn)
            {
                // boss의 이동 속도 설정
                bossCharacter = Cast<ABossApernia>(bossPawn);
                if (bossCharacter)
                {
                    
                    
                    bossCharacter->bossSwordComp->SetRelativeLocation(FVector(29.122834f, 84.073564f, 4.113887f));
                    bossCharacter->bossSwordComp->SetRelativeRotation(FRotator(4.826905f, 5.306981f, 8.324931f));
                 
                    UCharacterMovementComponent* movementComponent = bossCharacter->GetCharacterMovement();
                    if (movementComponent)
                    {
                        movementComponent->MaxWalkSpeed = 300.0f;  // MaxWalkSpeed를 300으로 설정
                    }
                }

                // 플레이어를 향해 포커스를 맞춤
                bossController->SetFocus(player);

                // 원형 이동을 위한 매개변수 정의
                float radius = 1000.0f;  // 반경은 그대로 유지
                float angularSpeed = 10.f; // 각속도 (속도는 유지)
                float time = GetWorld()->GetTimeSeconds();
                float timeOffset = 5.0f; // 위치 업데이트 간격 (초 단위)

                // 시간을 기반으로 각도 계산
                float angle = fmod((time + timeOffset) * angularSpeed, 360.0f);
                float angleRad = FMath::DegreesToRadians(angle);

                // 플레이어 주위의 원에서 목표 위치 계산
                FVector direction = FVector(FMath::Cos(angleRad), FMath::Sin(angleRad), 0.0f);
                FVector targetLocation = playerLocation + (direction * radius);

                // 목표 위치로 이동
                UAIBlueprintHelperLibrary::SimpleMoveToLocation(bossController, targetLocation);
            }
        }
    }

    // 태스크를 진행 중으로 설정
    bNotifyTick = true;
    return EBTNodeResult::InProgress;
}

void UTask_AttackWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    currentTime += DeltaSeconds;
    if (bossCharacter->bossDied == true)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }


    // 4초 동안 플레이어를 보며 대기
    if (currentTime < 3.0f)
    {
        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ACharacter* boss = Cast<ACharacter>(ControlledPawn);
               
            }
        }
    }

    // 3.1초가 지나면 SetFocus를 해제하고 태스크를 완료
    if (currentTime >= 2.5f)
    {
        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner()))
        {
            // 포커스를 해제합니다.
            bossController->ClearFocus(EAIFocusPriority::Gameplay);

            APawn* bossPawn = bossController->GetPawn();
            if (bossPawn)
            {
                // boss의 이동 속도 설정
                bossCharacter = Cast<ABossApernia>(bossPawn);
                if (bossCharacter)
                {
                    UCharacterMovementComponent* movementComponent = bossCharacter->GetCharacterMovement();
                    if (movementComponent)
                    {
                        movementComponent->MaxWalkSpeed = 600.0f;  // MaxWalkSpeed를 600으로 설정
                    }
                }
            }
        }
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        taskOnceMove = false;
    }
}
