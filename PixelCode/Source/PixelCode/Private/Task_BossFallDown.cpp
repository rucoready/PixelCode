// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_BossFallDown.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h" //캐릭터 임시
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "Boss/BossApernia.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"

UTask_BossFallDown::UTask_BossFallDown(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Boss Fall Down");


	bNotifyTick = true;
}

EBTNodeResult::Type UTask_BossFallDown::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	TickTask(OwnerComp, NodeMemory, 0.0f);



	return EBTNodeResult::InProgress;
}

void UTask_BossFallDown::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (currentTime == 0.0f)
	{
		animOnce = false;
	}
	currentTime += DeltaSeconds;
	// 애니메이션 실행
	if (currentTime < 1.0f && !animOnce)
	{
		AAIController* bossController = Cast<AAIController>(OwnerComp.GetOwner());
		APawn* ControlledPawn = bossController->GetPawn();
		if (ControlledPawn)
		{
			ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);
			
		}
		animOnce = true;
	}
	// 4.5초가 지나면 태스크 완료
	if (currentTime >= 3.5f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		currentTime = 0.0f; // currentTime 초기화
		animOnce = false;
	}
}
