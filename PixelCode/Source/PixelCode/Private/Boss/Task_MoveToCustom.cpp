// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task_MoveToCustom.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h"



UTask_MoveToCustom::UTask_MoveToCustom(FObjectInitializer const& ObjectInitializer)
{
    currentTime = 0.0f;
    escapeTime = 3.0f;
    bNotifyTick = true;
}

EBTNodeResult::Type UTask_MoveToCustom::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    currentTime = 0.0f;
    //TickTask(OwnerComp, NodeMemory, 0.0f);
    
    TickTask(OwnerComp, NodeMemory, 0.0f);
    //return EBTNodeResult::Succeeded;
    return EBTNodeResult::InProgress;
   
}

void UTask_MoveToCustom::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
    
    
    currentTime += DeltaSeconds;

    if (currentTime >= 5.f)
    {
        //UE_LOG(LogTemp, Warning, TEXT("I am working Now!"));
    }

    // 캐릭터 가져오기
    APixelCodeCharacter* PixelCodeCharacter = Cast<APixelCodeCharacter>(OwnerComp.GetAIOwner()->GetPawn());

    if (PixelCodeCharacter)
    {

        // 현재 캐릭터의 위치
        //FVector CurrentLocation = PixelCodeCharacter->GetActorLocation();

        // 캐릭터의 ForwardVector (전방 벡터) 방향으로 이동할 거리
        //float MoveDistance = 100.0f; // 예시로 100 유닛 이동

        // ForwardVector 방향으로 이동할 위치 계산
        //FVector NewLocation = CurrentLocation + PixelCodeCharacter->GetActorForwardVector() * MoveDistance;

        // 캐릭터 위치 변경
        //PixelCodeCharacter->SetActorLocation(NewLocation);
    }

    

    
}