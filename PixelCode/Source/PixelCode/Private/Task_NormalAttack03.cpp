// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_NormalAttack03.h"
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

UTask_NormalAttack03::UTask_NormalAttack03(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Normal Attack 03");


	bNotifyTick = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageObj(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AM_NormalSlash04_Montage.AM_NormalSlash04_Montage'"));
	if (MontageObj.Succeeded())
	{
		swordNormalAttack04 = MontageObj.Object;
	}
}

EBTNodeResult::Type UTask_NormalAttack03::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TickTask(OwnerComp, NodeMemory, 0.0f);
	animOnce = false;

    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

    int32 randomIndex = FMath::RandRange(0, foundCharacters.Num() - 1);
    player = Cast<APixelCodeCharacter>(foundCharacters[randomIndex]);

    if (player)
    {
        //플레이어의 위치를 얻어낸다
        playerLocation = player->GetActorLocation();
        //보스컨트롤러를 캐스팅
        ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
        if (bossController)
        {
            APawn* bossPawn = bossController->GetPawn();
            if (bossPawn)
            {

                // 방향 설정
                FVector direction = playerLocation - bossPawn->GetActorLocation();
                direction.Z = 0; // 보스가 수평으로만 회전하도록 Z축 회전 제거
                FRotator newRotation = direction.Rotation();
                bossPawn->SetActorRotation(newRotation);
            }
        }
    }
    ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
    if (bossController)
    {
        bossController->StopMovement();
        APawn* bossPawn = bossController->GetPawn();
        ABossApernia* boss = Cast<ABossApernia>(bossPawn);
        if (boss->GetMesh() && boss->GetMesh()->GetAnimInstance())
        {
            boss->bossSwordComp->SetRelativeLocation(FVector(24.794822f, 62.623682f, 20.052923f));
            boss->bossSwordComp->SetRelativeRotation(FRotator(13.210641f, 172.725964f, 184.233565f));
        }
    }
    return EBTNodeResult::InProgress;
}

void UTask_NormalAttack03::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    // 태스크 시작 시 초기화
    if (currentTime == 0.0f)
    {
        animOnce = false;
        directionSet = false; // 방향 설정 여부 플래그 초기화
        moveDirection = FVector::ZeroVector; // 초기 방향 설정
    }

    currentTime += DeltaSeconds;

    // 애니메이션 실행
    if (currentTime < 1.0f && !animOnce)
    {
        AAIController* bossController = Cast<AAIController>(OwnerComp.GetOwner());
        if (bossController)
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);

                if (swordNormalAttack04 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance())
                {
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();
                    boss->ServerRPC_NormalAttack03V1();
                    animOnce = true;
                }
            }
        }
    }

    ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
    if (bossController)
    {
        APawn* bossPawn = bossController->GetPawn();
        if (bossPawn)
        {
            playerLocation;
            FVector currentLocation = bossPawn->GetActorLocation();

            if (currentTime >= 1.1f && currentTime <= 1.3f)
            {
                // 1.1초에서 1.3초 사이에 플레이어의 위치를 다시 얻어와서 방향 설정
               
                if (player)
                {
                    playerLocation = player->GetActorLocation();
                }

                FVector direction = playerLocation - currentLocation;
                direction.Z = 0; // 수평으로만 회전하도록 Z축 회전 제거
                moveDirection = direction.GetSafeNormal(); // 이동 방향 업데이트

                FRotator newRotation = direction.Rotation();
                bossPawn->SetActorRotation(newRotation);

                // 방향 설정 완료
                directionSet = true;
            }
            else if (currentTime > 1.3f && directionSet)
            {
                // 1.3초 이후에는 저장된 방향으로 계속 이동
                FVector targetLocation = currentLocation + moveDirection * speed * DeltaSeconds * 4;
                bossPawn->SetActorLocation(targetLocation);
            }
            else
            {
                // 1.0초 전후의 이동 처리
                if (currentTime >= 0.0f && currentTime <= 0.1f)
                {
                    FVector targetLocation = currentLocation + bossPawn->GetActorForwardVector() * speed * DeltaSeconds * 4;
                    bossPawn->SetActorLocation(targetLocation);
                }
                else if (currentTime >= 0.1f && currentTime <= 0.4f)
                {
                    FVector targetLocation = currentLocation + bossPawn->GetActorForwardVector() * speed2 * DeltaSeconds * 4;
                    bossPawn->SetActorLocation(targetLocation);
                }
                else if (currentTime >= 0.7f && currentTime <= 0.9f)
                {
                    FVector targetLocation = currentLocation + bossPawn->GetActorForwardVector() * speed5 * DeltaSeconds * 4;
                    bossPawn->SetActorLocation(targetLocation);
                }
                else if (currentTime >= 0.9f && currentTime <= 1.1f)
                {
                    FVector targetLocation = currentLocation + bossPawn->GetActorForwardVector() * speed3 * DeltaSeconds * 4;
                    bossPawn->SetActorLocation(targetLocation);
                }
            }

            // 1.78초에서 2.1초 사이의 이동 처리
            if (currentTime >= 1.78f && currentTime <= 2.1f)
            {
                FVector targetLocation = currentLocation + bossPawn->GetActorForwardVector() * speed5 * DeltaSeconds * 4;
                bossPawn->SetActorLocation(targetLocation);
            }

            if (currentTime > 2.0)
            {
                bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
                if (bossController)
                {
                    bossPawn = bossController->GetPawn();
                    ABossApernia* boss = Cast<ABossApernia>(bossPawn);
                    if (boss->GetMesh() && boss->GetMesh()->GetAnimInstance())
                    {
                        boss->bossSwordComp->SetRelativeLocation(FVector(29.425722f, 55.060376f, 8.3646449f));
                        boss->bossSwordComp->SetRelativeRotation(FRotator(4.826905f, 1.306981f, 8.324931f));
                    }
                }
            }
        }
    }

    // 5초가 지나면 태스크 완료
    if (currentTime >= 2.2f)
    {
        
        animOnce = false;
        currentTime = 0.0f; // currentTime 초기화
        directionSet = false; // 방향 설정 여부 플래그 초기화
        moveDirection = FVector::ZeroVector; // 이동 방향 초기화
        normalAttack03 = false;


        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
        BlackboardComp = OwnerComp.GetBlackboardComponent();
        if (BlackboardComp)
        {
            BlackboardComp->SetValueAsBool(normalAttack03CoolTime.SelectedKeyName, normalAttack03);
        }

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
