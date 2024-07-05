// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task_JumpAttack01.h"
#include "Boss/BossAIController.h"
#include "Boss/BossApernia.h"
#include "Animation/AnimInstance.h"
#include "Player/PixelCodeCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"


UTask_JumpAttack01::UTask_JumpAttack01(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Jump Attack 01");
    
    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationFinish/AM_BossComboAttack02_Montage.AM_BossComboAttack02_Montage'"));
    if (montageObj.Succeeded())
    {
        jumpAttack01 = montageObj.Object;
    }
    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj2(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_Z1_Montage.AM_Z1_Montage'"));
    if (montageObj2.Succeeded())
    {
        jumpAttack03V2 = montageObj2.Object;
    }
    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj3(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_Z2_Montage.AM_Z2_Montage'"));
    if (montageObj3.Succeeded())
    {
        jumpAttack03V3 = montageObj3.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj4(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_Z3_Montage.AM_Z3_Montage'"));
    if (montageObj4.Succeeded())
    {
        jumpAttack03V4 = montageObj4.Object;
    }
    bNotifyTick = true;
}

EBTNodeResult::Type UTask_JumpAttack01::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    TickTask(OwnerComp, NodeMemory, 0.0f);
    animOnce = false;
    // 이동을 멈추기 위해 SimpleMoveToLocation 호출
    ABossAIController* BossAIController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
    if (BossAIController)
    {
        APawn* BossPawn = BossAIController->GetPawn();
        if (BossPawn)
        {
            BossAIController->StopMovement(); // 이동을 멈추도록 AI 컨트롤러에 지시
        }
    }

    return EBTNodeResult::InProgress;
    
    
}

void UTask_JumpAttack01::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    if (currentTime == 0.0f)
    {
        animOnce = false;
    }

    currentTime += DeltaSeconds;

    if (currentTime > 0.0 && currentTime < 1.3)
    {
        APixelCodeCharacter* const player = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
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
    if (currentTime > 2.3 && currentTime < 2.4)
    {
        APixelCodeCharacter* const player = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
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

    if (currentTime < 1.0f)
    {
        
        APixelCodeCharacter* const player = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        if (player)
        {
            playerLocation = player->GetActorLocation();
            OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), playerLocation);

            ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
            if (bossController)
            {
                APawn* bossPawn = bossController->GetPawn();
                if (bossPawn)
                {
                    FVector direction = playerLocation - bossPawn->GetActorLocation();
                    direction.Z = 0;
                    FRotator newRotation = direction.Rotation();
                    bossPawn->SetActorRotation(newRotation);
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
                ACharacter* boss = Cast<ACharacter>(ControlledPawn);

                if (jumpAttack03V2 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnceV2)
                {
                    //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                    boss->PlayAnimMontage(jumpAttack03V2);
                    animOnceV2 = true;

                }
            }
        }
    }
    if (currentTime > 1.0 && currentTime < 2.0)
    {


        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ACharacter* boss = Cast<ACharacter>(ControlledPawn);

                if (jumpAttack03V3 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnceV3)
                {
                    //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                    boss->PlayAnimMontage(jumpAttack03V3);
                    animOnceV3 = true;

                }
            }
        }
    }

    if (currentTime > 1.5 && currentTime < 2.2)
    {


        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ACharacter* boss = Cast<ACharacter>(ControlledPawn);

                if (jumpAttack03V4 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnceV4)
                {
                    //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                    boss->PlayAnimMontage(jumpAttack03V4);
                    animOnceV4 = true;

                }
            }
        }
    }

    if (currentTime > 2.3 && currentTime < 2.5)
    {


        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ACharacter* boss = Cast<ACharacter>(ControlledPawn);

                if (jumpAttack01 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnce)
                {
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();
                    boss->PlayAnimMontage(jumpAttack01);

                    animOnce = true;
                }
            }
        }
    }



    APixelCodeCharacter* const player = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (player)
    {
        playerLocation = player->GetActorLocation();
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), playerLocation);

        ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
        if (bossController)
        {

            APawn* bossPawn = bossController->GetPawn();
            if (bossPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(bossPawn);

                FVector bossLocation = boss->GetActorLocation();
                FVector bossForwardVector = boss->GetActorForwardVector();

                FVector bossSmashingLocation = bossLocation - FVector(0.0f, 0.0f, -100.0f);
                FVector bossGroundLocation = bossLocation - FVector(0.0f, 0.0f, boss->GetSimpleCollisionHalfHeight());
                FVector bossGroundLocation2 = bossLocation - FVector(0.0f, 0.0f, boss->GetSimpleCollisionHalfHeight());
                FVector bossGroundLocation3 = bossLocation - FVector(0.0f, 0.0f, boss->GetSimpleCollisionHalfHeight());

                FVector spawnLocation = bossGroundLocation + bossForwardVector * 200.0f;

                // -60도 회전 계산
                FRotator rotationMinus60 = boss->GetActorRotation();
                rotationMinus60.Yaw -= 15.0f;
                FVector directionMinus60 = rotationMinus60.Vector();
                FVector spawnLocation2 = bossGroundLocation2 + directionMinus60 * 200.0f;

                // +60도 회전 계산
                FRotator rotationPlus60 = boss->GetActorRotation();
                rotationPlus60.Yaw += 15.0f;
                FVector directionPlus60 = rotationPlus60.Vector();
                FVector spawnLocation3 = bossGroundLocation3 + directionPlus60 * 200.0f;

                // 현재 보스의 회전값을 가져옵니다.
                FRotator bossRotation = boss->GetActorRotation();

                // 현재 Pitch 축 값에 90도를 더한 회전값을 계산합니다.
                float newPitch = bossRotation.Pitch + 90.f;

                // Roll과 Yaw 축 값은 변하지 않도록 현재 값으로 설정합니다.
                float roll = bossRotation.Roll;
                float yaw = bossRotation.Yaw;

                // 새로운 회전값을 FRotator로 설정합니다.
                FRotator bossSmashingRotator(newPitch, yaw, roll);

                bossSmashingRotator.Yaw += -90.0f;
                

                

                

                if (currentTime > 2.0f && !onceNiagara01)
                {
                    //UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundImpactSwing, bossSmashingLocation, bossSmashingRotator, FVector(3.0f));
                    //UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundImpact02Niagara, spawnLocation, boss->GetActorRotation(), FVector(1.5f));
                    //UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundImpact01Niagara, spawnLocation2, rotationMinus60, FVector(1.5f));
                    //UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundImpact03Niagara, spawnLocation3, rotationPlus60, FVector(1.5f));

                    onceNiagara01 = true;
                    
                }
                if (currentTime > 3.2f &&!onceNiagara02)
                {
                    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundImpactV2Niagara, spawnLocation, boss->GetActorRotation(), FVector(2.0f));
                    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundImpactV3Niagara, spawnLocation2, rotationMinus60, FVector(2.0f));
                    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundImpactV4Niagara, spawnLocation3, rotationPlus60, FVector(2.0f));
                    onceNiagara02 = true;
                }

            }
        }
    }

    if (currentTime >= 4.0f)
    {
        ABossAIController* BossAIController = Cast<ABossAIController>(OwnerComp.GetAIOwner());

        
        animOnce = false;
        currentTime = 0.0f;
        onceNiagara01 = false;
        onceNiagara02 = false;
        jumpAttack1 = false;
        animOnceV2 = false;
        animOnceV3 = false;
        animOnceV4 = false;
        
        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
        BlackboardComp = OwnerComp.GetBlackboardComponent();
        if (BlackboardComp)
        {
            BlackboardComp->SetValueAsBool(jumpAttack1CoolTime.SelectedKeyName, jumpAttack1);
        }
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        
    }
}
