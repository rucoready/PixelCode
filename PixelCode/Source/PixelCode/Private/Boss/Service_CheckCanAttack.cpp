// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Service_CheckCanAttack.h"
#include "Player/PixelCodeCharacter.h" //캐릭터 임시
#include "Boss/BossApernia.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"

UService_CheckCanAttack::UService_CheckCanAttack()
{
	NodeName = TEXT("Check Can Attack");
}

void UService_CheckCanAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
    //UE_LOG(LogTemp, Warning, TEXT("DistanceBossToPixelCodeCharacter: %f"), distance);

    if (distance > 800)
    {
        canAttack = false;
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), canAttack);
    }
    else
    {
        canAttack = true;
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), canAttack);

    }

}
