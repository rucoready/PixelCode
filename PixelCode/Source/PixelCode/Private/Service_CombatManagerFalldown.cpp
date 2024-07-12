// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_CombatManagerFalldown.h"
#include "Player/PixelCodeCharacter.h" // 캐릭터 임시
#include "Boss/BossApernia.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"

UService_CombatManagerFalldown::UService_CombatManagerFalldown()
{
	NodeName = TEXT("Combat Manager Falldown");
}

void UService_CombatManagerFalldown::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // 시간을 업데이트
    currentTime += DeltaSeconds;

    // BossApernia 찾기
    ABossApernia* bossCharacter = Cast<ABossApernia>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossApernia::StaticClass()));
    if (bossCharacter)
    {
        //보스캐릭터의 bBossAttackFallDownAttack 변수가 true라면
        if (bossCharacter->bBossAttackFallDownAttack == true)
        {
            //bossFallDownOverlap를 true로 만들고
            bossFallDownOverlap = true;
            
            UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
            
            //bFallDown의 블랙보드값을 true로 변경한다
            BlackboardComp->SetValueAsBool(bFallDown.SelectedKeyName, bossFallDownOverlap);
        }
        
        
        return;
    }
}
