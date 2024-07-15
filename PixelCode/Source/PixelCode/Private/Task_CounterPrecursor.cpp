// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_CounterPrecursor.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h" //ĳ���� �ӽ�
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "Boss/BossApernia.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"

UTask_CounterPrecursor::UTask_CounterPrecursor(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Counter Precursor");

	bNotifyTick = true;
}

EBTNodeResult::Type UTask_CounterPrecursor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TickTask(OwnerComp, NodeMemory, 0.0f);
	animOnce = false;

    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

    int32 randomIndex = FMath::RandRange(0, foundCharacters.Num() - 1);
    player = Cast<APixelCodeCharacter>(foundCharacters[randomIndex]);

    if (player)
    {
        //�÷��̾��� ��ġ�� ����
        playerLocation = player->GetActorLocation();
        //������Ʈ�ѷ��� ĳ����
        ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
        if (bossController)
        {
            APawn* bossPawn = bossController->GetPawn();
            if (bossPawn)
            {

                // ���� ����
                FVector direction = playerLocation - bossPawn->GetActorLocation();
                direction.Z = 0; // ������ �������θ� ȸ���ϵ��� Z�� ȸ�� ����
                FRotator newRotation = direction.Rotation();
                bossPawn->SetActorRotation(newRotation);
            }
        }
    }

    return EBTNodeResult::InProgress;
}

void UTask_CounterPrecursor::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
    currentTime += DeltaSeconds;

    // �ִϸ��̼� ����
    if (currentTime < 1.0f && !animOnce)
    {
        AAIController* bossController = Cast<AAIController>(OwnerComp.GetOwner());
        if (bossController)
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);

                if (boss->GetMesh() && boss->GetMesh()->GetAnimInstance())
                {
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();
                    boss->ServerRPC_CounterPrecursor();
                    animOnce = true;
                }
            }
        }
    }

    // 3�ʰ� ������ �½�ũ �Ϸ�
    if (currentTime >= 2.0f)
    {
        animOnce = false;
        currentTime = 0.0f; // currentTime �ʱ�ȭ
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
