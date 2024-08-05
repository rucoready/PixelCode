// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BTService_FindPlayerLocation.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossAIController.h"


UBTService_FindPlayerLocation::UBTService_FindPlayerLocation()
{
	NodeName = TEXT("Tick Find Player Location");
	Interval = 1.0f; // Tick interval (1 second)

	bSearchRandom = false;
	SearchRadius = 1000.0f; // Default search radius
}

void UBTService_FindPlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


    // 블랙보드 컴포넌트 가져오기
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return;
    }

    // 플레이어 찾기
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!PlayerCharacter)
    {
        return;
    }

    // 현재 관찰 대상 설정 (여기서는 플레이어)
//     AActor* Actor = OwnerComp.GetOwner();
//     if (Actor)
//     {
//         // Actor가 ThirdPerson인지 확인
//         if (Actor->GetActorLabel().Contains("ThirdPerson"))
//         {
//             // Actor의 방향을 플레이어 방향으로 설정
//             FVector PlayerDirection = PlayerCharacter->GetActorLocation() - Actor->GetActorLocation();
//             PlayerDirection.Normalize();
//             FRotator PlayerRotation = PlayerDirection.Rotation();
//             Actor->SetActorRotation(PlayerRotation);
//         }
//     }
}
