// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task_JumpAttack03.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Boss/BossApernia.h"

UTask_JumpAttack03::UTask_JumpAttack03(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Jump Attack 03");

    
    bNotifyTick = true;
}

EBTNodeResult::Type UTask_JumpAttack03::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    TickTask(OwnerComp, NodeMemory, 0.0f);

    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

    int32 randomIndex = FMath::RandRange(0, foundCharacters.Num() - 1);
    player = Cast<APixelCodeCharacter>(foundCharacters[randomIndex]);
    
    
    
    ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
    if (bossController)
    {
        bossController->StopMovement();
        if (ABossApernia* boss = Cast<ABossApernia>(OwnerComp.GetAIOwner()->GetPawn()))
        {
                    
            boss->bossSwordComp->SetRelativeLocation(FVector(17.137708f, 57.508425f, 23.246429f));
            boss->bossSwordComp->SetRelativeRotation(FRotator(28.852794f, 169.726741f, 183.362852f));
            boss->ServerRPC_JumpAttack03SwordPositionSet();
        }
            
    }
    

    return EBTNodeResult::InProgress;
}

void UTask_JumpAttack03::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // 태스크 시작 시 초기화
    if (currentTime == 0.0f)
    {
        animOnce = false;
    }
    currentTime += DeltaSeconds;

    
    if (currentTime > 0.0 && currentTime < 0.2)
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
    if (currentTime > 0.9 && currentTime < 1.2)
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
    if (currentTime > 0.3f)
    {
        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);

                if (boss->GetMesh() && !jumpOnce)
                {
                    // LaunchCharacter를 호출하여 보스를 z축으로 30만큼 날려줌
                    FVector LaunchVelocity(0, 0, 1400);

                    //boss->LaunchCharacter(LaunchVelocity, true, true);
                   
                    jumpOnce = true;

                }
            }
        }
    }

    if (currentTime > 0.0 && currentTime < 1.0)
    {


        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);

                if (boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnceV3)
                {
                    //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                    boss->ServerRPC_JumpAttack03V3();
                    animOnceV3 = true;

                }
            }
        }
    }

    if (currentTime > 0.8 && currentTime < 1.8)
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

                    boss->ServerRPC_JumpAttack03V1();
                    animOnce = true;

                }
            }
        }
    }
    if (currentTime > 1.5 && currentTime < 2.1)
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

                    boss->ServerRPC_JumpAttack03V2();
                    animOnceV2 = true;

                }
            }
        }
    }
    if (currentTime > 2.1f)
    {
        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);

                if (boss->GetMesh() && !downOnce)
                {
                    // LaunchCharacter를 호출하여 보스를 z축으로 30만큼 날려줌
                    FVector LaunchVelocity(0, 0, -4000);

                    //boss->LaunchCharacter(LaunchVelocity, true, true);

                    downOnce = true;

                }
            }
        }
    }

    if (currentTime > 1.7f && !niagaraOnce)
    {
        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);
                
                boss->ServerRPC_SpawnJumpAttackNiagara3V1();
                boss->ServerRPC_SpawnBoundCollision();
                niagaraOnce = true;

                if (player && !cameraMovingOnce)
                {
                    APlayerController* pc = player->GetController<APlayerController>();
                    if (pc != nullptr)
                    {
                        boss->ServerRPC_JumpAttack03CameraShake();

                    }
                }

            }
        }
    }

    if (currentTime > 3.8f)
    {

        if (ABossApernia* boss = Cast<ABossApernia>(OwnerComp.GetAIOwner()->GetPawn()))
        {

            boss->bossSwordComp->SetRelativeLocation(FVector(29.425722f, 55.060376f, 8.3646449f));
            boss->bossSwordComp->SetRelativeRotation(FRotator(4.826905f, 1.306981f, 8.324931f));
            boss->ServerRPC_JumpAttack03SwordPositionReSet();
        }
    }
    


    // 1.8초가 지나면 태스크 완료
    if (currentTime >= 3.5f)
    {
        currentTime = 0.0f; // currentTime 초기화
        jumpOnce = false;
        downOnce = false;
        niagaraOnce = false;
        animOnce = false;
        animOnceV2 = false;
        animOnceV3 = false;
        cameraMovingOnce = false;
        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
        BlackboardComp = OwnerComp.GetBlackboardComponent();
        if (BlackboardComp)
        {
            BlackboardComp->SetValueAsBool(jumpAttack3CoolTime.SelectedKeyName, jumpAttack3);
        }
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        
    }
}