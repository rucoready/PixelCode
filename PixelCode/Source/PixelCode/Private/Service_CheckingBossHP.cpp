// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_CheckingBossHP.h"
#include "Player/PixelCodeCharacter.h" //캐릭터 임시
#include "Boss/BossApernia.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"

UService_CheckingBossHP::UService_CheckingBossHP()
{
	NodeName = TEXT("Check Boss Current HP");
}

void UService_CheckingBossHP::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    // BossApernia 찾기
    ABossApernia* bossCharacter = Cast<ABossApernia>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossApernia::StaticClass()));
    if (!bossCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find boss character."));
        return;
    }
    if (bossCharacter)
    {
        if (bossCharacter->bossCurrentHP <= 40000.0f)
        {
            over1Phase = true;
            OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), over1Phase);
        }
        else
        {
            over1Phase = false;
            OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), over1Phase);
        }
        
    }
}
