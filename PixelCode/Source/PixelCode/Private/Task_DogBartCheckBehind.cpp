
#include "Task_DogBartCheckBehind.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Player/PixelCodeCharacter.h"
#include "DogBart.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DogBartAIController.h"

UTask_DogBartCheckBehind::UTask_DogBartCheckBehind(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Check DogBart Behind");
    currentTime = 0.0f;

    bNotifyTick = true;
}

EBTNodeResult::Type UTask_DogBartCheckBehind::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 작업이 실행될 때마다 currentTime을 0으로 리셋
    currentTime = 0.0f;
    CanAttack = false;

    dogBart = Cast<ADogBart>(OwnerComp.GetAIOwner()->GetPawn());

    return EBTNodeResult::InProgress;
    

}

void UTask_DogBartCheckBehind::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
    currentTime += DeltaSeconds;

    if (currentTime < 0.3f)
    {
        TArray<AActor*> foundDogBart;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADogBart::StaticClass(), foundDogBart);

        dogBart = Cast<ADogBart>(OwnerComp.GetAIOwner()->GetPawn());

        dogLoc = dogBart->GetActorLocation();
        FVector gruxForward = dogBart->GetActorForwardVector();

        TArray<AActor*> foundCharacters;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);
        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

        const int32 numTraces = 40; // 추적 수
        const float angleStep = 360.0f / numTraces; // 360도를 numTraces로 나눈 각도 간격

        for (AActor* actor : foundCharacters)
        {
            actorLoc = actor->GetActorLocation();
            FVector toActor = (actorLoc - dogLoc).GetSafeNormal();
            distance = FVector::Dist(dogLoc, actorLoc);
            float dotProduct = FVector::DotProduct(gruxForward, toActor);
            float angle = FMath::Acos(dotProduct) * (180.0f / PI);

            // 360도 방향으로 선을 쏘는 루프
            for (int32 i = 0; i < numTraces; ++i)
            {
                float currentAngle = i * angleStep; // 현재 각도
                FRotator rotation = FRotator(0.0f, currentAngle, 0.0f); // 각도를 회전값으로 변환
                FVector direction = rotation.RotateVector(gruxForward); // 방향 벡터 계산

                FVector traceEnd = dogLoc + direction * 1500.0f; // 추적 끝 지점
                FHitResult hitResult;
                FCollisionQueryParams queryParams;
                queryParams.AddIgnoredActor(dogBart);

                bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, dogLoc, traceEnd, ECC_Visibility, queryParams);

                //DrawDebugLine(GetWorld(), dogLoc, traceEnd, FColor::Blue, false, 0.1f, 0, 1.0f);

                if (bHit && hitResult.GetActor())
                {
                    FString actorName = hitResult.GetActor()->GetName();
                    if (actorName.Contains("Player"))
                    {
                        player = Cast<APixelCodeCharacter>(hitResult.GetActor());
                        CanAttack = true;
                        BlackboardComp->SetValueAsBool(canAttack.SelectedKeyName, CanAttack);
                        // 맞은 플레이어의 위치를 Blackboard에 저장
                        BlackboardComp->SetValueAsVector(detectedPlayer.SelectedKeyName, player->GetActorLocation());

                        // 맞은 플레이어를 쳐다보게 보간
                        FVector playerLoc = player->GetActorLocation();
                        FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(dogLoc, playerLoc);
                        FRotator newRot = FMath::RInterpTo(dogBart->GetActorRotation(), targetRot, DeltaSeconds, 7.0f);
                        dogBart->SetActorRotation(newRot);
                    }
                }
            }
        }
    }

    // 2.1초가 지나면 태스크 완료
    if (currentTime >= 0.3f)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
