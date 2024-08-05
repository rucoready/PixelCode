#include "Boss/Service_CalculateDistanceBetween.h"
#include "Player/PixelCodeCharacter.h" // 캐릭터 임시
#include "Boss/BossApernia.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"

UService_CalculateDistanceBetween::UService_CalculateDistanceBetween()
{
    NodeName = TEXT("Calculate Between Boss To Player");
    currentTime = 0.0f;
    timeToSelectPlayer = 10.0f; // 플레이어를 선택할 주기
}

void UService_CalculateDistanceBetween::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // 시간을 업데이트
    currentTime += DeltaSeconds;

    // BossApernia 찾기
    ABossApernia* bossCharacter = Cast<ABossApernia>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossApernia::StaticClass()));
    if (!bossCharacter)
    {
        return;
    }

    // 일정 시간이 경과하면 새로운 플레이어를 선택
    if (currentTime >= timeToSelectPlayer)
    {
        // 시간을 리셋
        currentTime = 0.0f;

        // 플레이어 캐릭터를 무작위로 선택
        TArray<AActor*> foundCharacters;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

        if (foundCharacters.Num() > 0)
        {
            int32 randomIndex = FMath::RandRange(0, foundCharacters.Num() - 1);
            player = Cast<APixelCodeCharacter>(foundCharacters[randomIndex]);

            // 선택된 플레이어의 위치를 로그로 출력
            if (player)
            {
				FVector playerLocation = player->GetActorLocation();
				
            }
        }
    }
    
    if (player)
    {
        // BossApernia와 PixelCodeCharacter 사이의 거리 계산
        float distance = FVector::Distance(bossCharacter->GetActorLocation(), player->GetActorLocation());

        // 블랙보드에 거리 값을 저장
        OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), distance);
    }
}
