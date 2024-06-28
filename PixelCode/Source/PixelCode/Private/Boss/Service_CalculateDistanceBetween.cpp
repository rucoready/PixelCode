// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Service_CalculateDistanceBetween.h"
#include "Player/PixelCodeCharacter.h" //캐릭터 임시
#include "Boss/BossApernia.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"

UService_CalculateDistanceBetween::UService_CalculateDistanceBetween()
{
	NodeName = TEXT("Calculate Between Boss To Player");
}

void UService_CalculateDistanceBetween::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    // BossApernia 찾기
    ABossApernia* bossCharacter = Cast<ABossApernia>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossApernia::StaticClass()));
    if (!bossCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find boss character."));
        return;
    }

    // PixelCodeCharacter 찾기
    APixelCodeCharacter* pixelCharacter = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!pixelCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find PixelCodeCharacter."));
        return;
    }

    

    // BossApernia와 PixelCodeCharacter 사이의 거리 계산
    float distance = FVector::Distance(bossCharacter->GetActorLocation(), pixelCharacter->GetActorLocation());
  

    // 블랙보드에 거리 값을 저장
    OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), distance);

    
    
    
}
