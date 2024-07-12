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
        jumpAttack01V1 = montageObj.Object;
    }
    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj2(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_Z1_Montage.AM_Z1_Montage'"));
    if (montageObj2.Succeeded())
    {
        jumpAttack01V2 = montageObj2.Object;
    }
    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj3(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_Z2_Montage.AM_Z2_Montage'"));
    if (montageObj3.Succeeded())
    {
        jumpAttack01V3 = montageObj3.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj4(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_Z3_Montage.AM_Z3_Montage'"));
    if (montageObj4.Succeeded())
    {
        jumpAttack01V4 = montageObj4.Object;
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

    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

    int32 randomIndex = FMath::RandRange(0, foundCharacters.Num() - 1);
    player = Cast<APixelCodeCharacter>(foundCharacters[randomIndex]);



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

    

    if (currentTime > 0.0 && currentTime < 0.1)
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
    if (currentTime > 0.9 && currentTime < 0.95)
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
    //set rotation / location
    if (currentTime > 2.3 && currentTime < 2.4)
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

    
    if (currentTime > 0.0 && currentTime < 1.0)
    {


        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);

                if (jumpAttack01V2 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnceV2)
                {
                    //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                    boss->ServerRPC_JumpAttack01V2();
                    animOnceV2 = true;

                }

                if (ABossApernia* bossComponent = Cast<ABossApernia>(OwnerComp.GetAIOwner()->GetPawn()))
                {
                    
                    boss->ServerRPC_SpawnJumpAttackSwordPositionSet();

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
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);

                if (jumpAttack01V3 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnceV3)
                {
                    //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                    boss->ServerRPC_JumpAttack01V3();
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
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);

                if (jumpAttack01V4 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnceV4)
                {
                    //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                    boss->ServerRPC_JumpAttack01V4();
                    animOnceV4 = true;
                    boss->ServerRPC_SpawnJumpAttackSwordPositionSet2();

                }
            }
        }
        if (ABossApernia* bossComponent = Cast<ABossApernia>(OwnerComp.GetAIOwner()->GetPawn()))
        {
            
            bossComponent->bossSwordComp->SetRelativeLocation(FVector(-9.262842f, 91.403065f, -6.445372f));
            bossComponent->bossSwordComp->SetRelativeRotation(FRotator(70.003781f, 182.098563f, 218.299690f));
            

        }
    }

    if (currentTime > 2.3 && currentTime < 2.5)
    {


        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);

                if (jumpAttack01V1 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnce)
                {
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();
                    boss->ServerRPC_JumpAttack01V1();

                    animOnce = true;
                }
            }
        }
    }



   
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


                if (currentTime > 2.0f && !onceNiagara01)
                {
                    
                    onceNiagara01 = true;
                    
                }
                if (currentTime > 3.2f &&!onceNiagara02)
                {
                    boss->ServerRPC_SpawnJumpAttackNiagara();
                    onceNiagara02 = true;
                }

            }
        }
    }

    if (currentTime >= 4.0f)
    {
        ABossAIController* BossAIController = Cast<ABossAIController>(OwnerComp.GetAIOwner());

        if (ABossApernia* bossComponent = Cast<ABossApernia>(OwnerComp.GetAIOwner()->GetPawn()))
        {

            bossComponent->bossSwordComp->SetRelativeLocation(FVector(29.122834f, 84.073564f, 4.113887f));
            bossComponent->bossSwordComp->SetRelativeRotation(FRotator(4.826905f, 1.306981f, 8.324931f));
            

        }
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
