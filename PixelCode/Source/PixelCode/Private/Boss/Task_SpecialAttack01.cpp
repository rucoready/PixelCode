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

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationFinish/AM_BossJumpAttack01.AM_BossJumpAttack01'"));
    if (montageObj.Succeeded())
    {
        jumpAttack02 = montageObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj2(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_NormalSlashV5_Montage.AM_NormalSlashV5_Montage'"));
    if (montageObj2.Succeeded())
    {
        jumpAttack02V2 = montageObj2.Object;
    }
    bNotifyTick = true;
}

EBTNodeResult::Type UTask_SpecialAttack01::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    TickTask(OwnerComp, NodeMemory, 0.0f);

    
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

    return EBTNodeResult::InProgress;
}

void UTask_SpecialAttack01::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // 태스크 시작 시 초기화
    if (currentTime == 0.0f)
    {
        animOnce = false;
    }


    if (currentTime > 0.0 && currentTime < 0.1)
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

    if (currentTime > 1.9 && currentTime < 2.0)
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

    if (currentTime > 0.0 && currentTime < 2.0)
    {


        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ACharacter* boss = Cast<ACharacter>(ControlledPawn);

                if (jumpAttack02V2 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnceV2)
                {
                    //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                    boss->PlayAnimMontage(jumpAttack02V2);
                    animOnceV2 = true;

                }
            }
        }
    }

    if (currentTime > 2.1 && currentTime < 3.2)
    {
        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ACharacter* boss = Cast<ACharacter>(ControlledPawn);

                if (jumpAttack02 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnce)
                {
                    //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                    boss->PlayAnimMontage(jumpAttack02);
                    animOnce = true;


                }
            }
        }
    }

    


    currentTime += DeltaSeconds;
    if (currentTime > 2.3f)
    {
        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ACharacter* boss = Cast<ACharacter>(ControlledPawn);

                if (boss->GetMesh()&& !jumpOnce)
                {
                    // LaunchCharacter를 호출하여 보스를 z축으로 30만큼 날려줌
                    FVector LaunchVelocity(0, 0, 900);

                    boss->LaunchCharacter(LaunchVelocity, true, true);
                    UE_LOG(LogTemp, Warning, TEXT("Launch Characters22222!"));
                    jumpOnce = true;

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
                ACharacter* boss = Cast<ACharacter>(ControlledPawn);

                if (boss->GetMesh() && !jumpOnce2)
                {
                    // LaunchCharacter를 호출하여 보스를 z축으로 30만큼 날려줌
                    FVector LaunchVelocity(0, 0, -4000);

                    boss->LaunchCharacter(LaunchVelocity, true, true);
                    UE_LOG(LogTemp, Warning, TEXT("Launch Characters!"));
                    jumpOnce2 = true;

                }
            }
        }
    }
    ////////////////////////////////////////////////////////////
    //첫번쨰
    if (currentTime > 3.6f && !jumpNiagara1)
    {
        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ACharacter* boss = Cast<ACharacter>(ControlledPawn);

                FVector bossLocation = boss->GetActorLocation();
                FVector bossForwardVector = boss->GetActorForwardVector();

                FVector bossSmashingLocation = bossLocation - FVector(0.0f, 0.0f, 500.0f);

                // 현재 보스의 회전값을 가져옵니다.
                FRotator bossRotation = boss->GetActorRotation();

                // 현재 Pitch 축 값에 90도를 더한 회전값을 계산합니다.
                float newPitch = bossRotation.Pitch + 270.f;

                // Roll과 Yaw 축 값은 변하지 않도록 현재 값으로 설정합니다.
                float roll = bossRotation.Roll;
                float yaw = bossRotation.Yaw;

                // 새로운 회전값을 FRotator로 설정합니다.
                FRotator bossSmashingRotator(newPitch, yaw, roll);

                bossSmashingRotator.Yaw += -90.0f;

                UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), jumpSwing2, bossSmashingLocation, bossSmashingRotator, FVector(3.0f));
                jumpNiagara1 = true;
            }
        }
    }
    //두번쨰
    if (currentTime > 3.7f && !jumpNiagara2)
    {
        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ACharacter* boss = Cast<ACharacter>(ControlledPawn);

                FVector bossLocation = boss->GetActorLocation();
                FVector bossForwardVector = boss->GetActorForwardVector();

                FVector bossSmashingLocation = bossLocation - FVector(0.0f, 0.0f, 500.0f);

                // 현재 보스의 회전값을 가져옵니다.
                FRotator bossRotation = boss->GetActorRotation();

                // 현재 Pitch 축 값에 90도를 더한 회전값을 계산합니다.
                float newPitch = bossRotation.Pitch + -180.f;

                // Roll과 Yaw 축 값은 변하지 않도록 현재 값으로 설정합니다.
                float roll = bossRotation.Roll;
                float yaw = bossRotation.Yaw;

                // 새로운 회전값을 FRotator로 설정합니다.
                FRotator bossSmashingRotator(newPitch, yaw, roll);

                bossSmashingRotator.Yaw += -90.0f;

                UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), jumpSwing3, bossSmashingLocation, bossSmashingRotator, FVector(3.0f));
                jumpNiagara2 = true;
            }
        }
    }
    //세번쨰
    if (currentTime > 3.9f && !jumpNiagara3)
    {
        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ACharacter* boss = Cast<ACharacter>(ControlledPawn);

                FVector bossLocation = boss->GetActorLocation();
                FVector bossForwardVector = boss->GetActorForwardVector();

                FVector bossSmashingLocation = bossLocation - FVector(0.0f, 0.0f, 200.0f);

                // 현재 보스의 회전값을 가져옵니다.
                FRotator bossRotation = boss->GetActorRotation();

                // 현재 Pitch 축 값에 90도를 더한 회전값을 계산합니다.
                float newPitch = bossRotation.Pitch + -180.f;

                // Roll과 Yaw 축 값은 변하지 않도록 현재 값으로 설정합니다.
                float roll = bossRotation.Roll;
                float yaw = bossRotation.Yaw;

                // 새로운 회전값을 FRotator로 설정합니다.
                FRotator bossSmashingRotator(newPitch, yaw, roll);

                bossSmashingRotator.Yaw += -90.0f;

                UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), jumpSwing3, bossSmashingLocation, bossSmashingRotator, FVector(3.0f));
                jumpNiagara3 = true;
            }
        }
    }
    

    // 1.8초가 지나면 태스크 완료
    if (currentTime >= 5.1f)
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
        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
        BlackboardComp = OwnerComp.GetBlackboardComponent();
        if (BlackboardComp)
        {
            BlackboardComp->SetValueAsBool(jumpAttack2CoolTime.SelectedKeyName, jumpAttack2);
        }
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        

    }
}
