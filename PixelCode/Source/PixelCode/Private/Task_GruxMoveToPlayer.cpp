// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_GruxMoveToPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GruxAIController.h"
#include "Grux.h"
#include "Player/PixelCodeCharacter.h" //캐릭터 임시
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UTask_GruxMoveToPlayer::UTask_GruxMoveToPlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Grux Move To Player");


	bNotifyTick = true;
}

EBTNodeResult::Type UTask_GruxMoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TickTask(OwnerComp, NodeMemory, 0.0f);
	return EBTNodeResult::InProgress;
}

void UTask_GruxMoveToPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    //그럭스 찾기
    AGrux* grux = Cast<AGrux>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrux::StaticClass()));
    FVector gruxLoc = grux->GetActorLocation();
    FVector gruxForward = grux->GetActorForwardVector();
    
    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);
    float betweenSize = FVector::Dist2D(gruxLoc, actorLoc);
   

    for (AActor* actor : foundCharacters)
    {
        actorLoc = actor->GetActorLocation();
        
        if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
        {
            AGruxAIController* gruxController = Cast<AGruxAIController>(OwnerComp.GetAIOwner());
            if (gruxController)
            {
                if (betweenSize > stopDistance)
                {
                    gruxController->MoveToLocation(actorLoc);
                }
                else
                {
                    gruxController->StopMovement();
                    FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
                }
                
 
            }
        }
    }
}
