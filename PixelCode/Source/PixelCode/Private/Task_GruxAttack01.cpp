// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_GruxAttack01.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h" //캐릭터 임시
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "Boss/BossApernia.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Grux.h"
#include "GruxAIController.h"
#include "GameFramework/Character.h"

UTask_GruxAttack01::UTask_GruxAttack01(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Attack01");


	bNotifyTick = true;
}

EBTNodeResult::Type UTask_GruxAttack01::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TickTask(OwnerComp, NodeMemory, 0.0f);
    currentTime = 0.0f;

    grux = Cast<AGrux>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrux::StaticClass()));
   
//     if (grux)
//     {
//         grux->bPlayAttack = true;
//         UE_LOG(LogTemp, Warning, TEXT("3232323232"));
//     }
    grux->attackSensing = true;

	return EBTNodeResult::InProgress;

}

void UTask_GruxAttack01::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	currentTime += DeltaSeconds;
    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

    int32 randomIndex = FMath::RandRange(0, foundCharacters.Num() - 1);
    player = Cast<APixelCodeCharacter>(foundCharacters[randomIndex]);
    if (currentTime > 0.0f && currentTime < 0.8f)
    {
        if (player)
        {
            playerLocation = player->GetActorLocation();
            //보스컨트롤러를 캐스팅
            AGruxAIController* gruxController = Cast<AGruxAIController>(OwnerComp.GetAIOwner());
            if (gruxController)
            {
                APawn* gruxPawn = gruxController->GetPawn();
                if (gruxPawn)
                {

                    // 방향 설정
                    FVector direction = playerLocation - gruxPawn->GetActorLocation();
                    direction.Z = 0; // 보스가 수평으로만 회전하도록 Z축 회전 제거
                    FRotator newRotation = direction.Rotation();
                    gruxPawn->SetActorRotation(newRotation);
                }
            }
        }
    }
	if (currentTime > 0.0f && currentTime < 0.2f)
	{
        AGruxAIController* gruxController = Cast<AGruxAIController>(OwnerComp.GetOwner());
        if (gruxController)
        {
            APawn* ControlledPawn = gruxController->GetPawn();
            if (ControlledPawn)
            {
                grux = Cast<AGrux>(ControlledPawn);

                if (grux->GetMesh() && grux->GetMesh()->GetAnimInstance() && !animOnce)
                {
                    UAnimInstance* AnimInstance = grux->GetMesh()->GetAnimInstance();
                    grux->ServerRPC_Attack01();
                    animOnce = true;
                }
            }
        }
	}
    if (currentTime >= 3.5f)
    {
        animOnce = false;
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
