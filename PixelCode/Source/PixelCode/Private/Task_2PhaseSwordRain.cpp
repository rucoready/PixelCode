// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_2PhaseSwordRain.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BossFloor.h"
#include "Math/UnrealMathUtility.h"
#include "Boss/BossApernia.h"
#include "WarningCircleDecal.h"

UTask_2PhaseSwordRain::UTask_2PhaseSwordRain(FObjectInitializer const& ObjectInitializer)
{
}

EBTNodeResult::Type UTask_2PhaseSwordRain::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type();
}

void UTask_2PhaseSwordRain::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}
