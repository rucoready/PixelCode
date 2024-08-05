// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task_SpecialAttack01.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossApernia.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

UTask_SpecialAttack01::UTask_SpecialAttack01(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Jump Attack 02");

    
    bNotifyTick = true;
}

EBTNodeResult::Type UTask_SpecialAttack01::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    TickTask(OwnerComp, NodeMemory, 0.0f);

    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

    int32 randomIndex = FMath::RandRange(0, foundCharacters.Num() - 1);
    player = Cast<APixelCodeCharacter>(foundCharacters[randomIndex]);

    

    return EBTNodeResult::InProgress;
}

void UTask_SpecialAttack01::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // 태스크 시작 시 초기화
    if (currentTime == 0.0f)
    {
        animOnce = false;
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
                    bossController->StopMovement();
                    // 방향 설정
                    FVector direction = playerLocation - bossPawn->GetActorLocation();
                    direction.Z = 0; // 보스가 수평으로만 회전하도록 Z축 회전 제거
                    FRotator newRotation = direction.Rotation();
                    bossPawn->SetActorRotation(newRotation);
                }
            }
        }
    }
    currentTime += DeltaSeconds;
    // 애니메이션 실행
    if (currentTime < 1.0f && !animOnceV3)
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
                    animOnceV3 = true;
                }
            }
        }
    }



    if (currentTime > 1.5 && currentTime < 1.6)
    {
       
        if (player)
        {
            //플레이어의 위치를 얻어낸다
            playerLocation = player->GetActorLocation();
            //보스컨트롤러를 캐스팅
            ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
            if (bossController)
            {
                bossController->StopMovement();
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
    }

    if (currentTime > 3.4 && currentTime < 3.5)
    {
        
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
    }

    if (currentTime > 1.5 && currentTime < 3.5)
    {


        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);

                if (boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnceV2)
                {
                    //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                    if (boss->bossHitCounterAttack == false)
                    {
                        boss->ServerRPC_JumpAttack02V2();
                        animOnceV2 = true;
                    }
                    

                }
            }
        }
    }

    if (currentTime > 3.6 && currentTime < 4.7)
    {
        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);

                if (boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnce)
                {
                    //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                    if (boss->bossHitCounterAttack == false)
                    {
                        boss->ServerRPC_JumpAttack02V1();
                        animOnce = true;
                    }
                    


                }
            }
        }
    }

    


    
    if (currentTime > 3.8f)
    {
        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);

                if (boss->GetMesh()&& !jumpOnce)
                {
                    if (boss->bossHitCounterAttack == false)
                    {
                        // LaunchCharacter를 호출하여 보스를 z축으로 30만큼 날려줌
                        FVector LaunchVelocity(0, 0, 900);

                        boss->LaunchCharacter(LaunchVelocity, true, true);
                        jumpOnce = true;
                    }
                    

                }
            }
        }
    }
    if (currentTime > 5.3f)
    {
        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);

                if (boss->GetMesh() && !jumpOnce2)
                {
                    // LaunchCharacter를 호출하여 보스를 z축으로 30만큼 날려줌
                    FVector LaunchVelocity(0, 0, -4000);
                    if (boss->bossHitCounterAttack == false)
                    {
                        boss->LaunchCharacter(LaunchVelocity, true, true);
                        jumpOnce2 = true;
                    }
                    

                }
            }
        }
    }
    ////////////////////////////////////////////////////////////
    //첫번쨰
    if (currentTime > 5.1f && !jumpNiagara1)
    {
        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);
                if (boss->bossHitCounterAttack == false)
                {
                    boss->ServerRPC_SpawnJumpAttackNiagara2V1();
                    jumpNiagara1 = true;
                }
                
            }
        }
    }
    //두번쨰
    if (currentTime > 5.2f && !jumpNiagara2)
    {
        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);
                if (boss->bossHitCounterAttack == false)
                {
                    boss->ServerRPC_SpawnJumpAttackNiagara2V2();
                    jumpNiagara2 = true;
                }
                
            }
        }
    }
    //세번쨰
    if (currentTime > 5.4f && !jumpNiagara3)
    {
        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);
                if (boss->bossHitCounterAttack == false)
                {
                    

                    boss->ServerRPC_SpawnJumpAttackNiagara2V3();
                    jumpNiagara3 = true;
                }
                
            }
        }
    }
    if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
    {
        APawn* ControlledPawn = bossController->GetPawn();
        if (ControlledPawn)
        {
            ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);
            if (boss->bossHitCounterAttack == true)
            {
                currentTime = 0.0f; // currentTime 초기화
                jumpOnce = false;
                jumpOnce2 = false;
                jumpNiagara1 = false;
                jumpNiagara2 = false;
                jumpNiagara3 = false;
                jumpAttack2 = false;
                animOnce = false;
                animOnceV2 = false;
                animOnceV3 = false;
                UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
                BlackboardComp = OwnerComp.GetBlackboardComponent();
                if (BlackboardComp)
                {
                    BlackboardComp->SetValueAsBool(jumpAttack2CoolTime.SelectedKeyName, jumpAttack2);
                }
                FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

            }
        }
    }
    // 1.8초가 지나면 태스크 완료
    if (currentTime >= 6.6f)
    {
        currentTime = 0.0f; // currentTime 초기화
        jumpOnce = false;
        jumpOnce2 = false;
        jumpNiagara1 = false;
        jumpNiagara2 = false;
        jumpNiagara3 = false;
        jumpAttack2 = false;
        animOnce = false;
        animOnceV2 = false;
        animOnceV3 = false;
        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
        BlackboardComp = OwnerComp.GetBlackboardComponent();
        if (BlackboardComp)
        {
            BlackboardComp->SetValueAsBool(jumpAttack2CoolTime.SelectedKeyName, jumpAttack2);
        }
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        

    }
}
