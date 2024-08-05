// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Service_ShortOrFaR.h"
#include "Player/PixelCodeCharacter.h" //캐릭터 임시
#include "Boss/BossApernia.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"

UService_ShortOrFaR::UService_ShortOrFaR()
{
	NodeName = TEXT("Check Short Or Far");
}

void UService_ShortOrFaR::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // BossApernia 찾기
    ABossApernia* bossCharacter = Cast<ABossApernia>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossApernia::StaticClass()));
    if (!bossCharacter)
    {
        return;
    }

    // PixelCodeCharacter 찾기
    APixelCodeCharacter* pixelCharacter = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!pixelCharacter)
    {
        return;
    }


    // BossApernia와 PixelCodeCharacter 사이의 거리 계산
    float distance = FVector::Distance(bossCharacter->GetActorLocation(), pixelCharacter->GetActorLocation());
    if (distance > 2000)
    {
        bShort = false;
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bShort);

    }
    else if (distance < 2000)
    {
        bShort = true;
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bShort);
    }
    

    
}
