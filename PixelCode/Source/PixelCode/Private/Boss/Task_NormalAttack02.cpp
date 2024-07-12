// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task_NormalAttack02.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossApernia.h"

UTask_NormalAttack02::UTask_NormalAttack02(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("NormalAttack 02");

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationFinish/AS_BossLongSwing_Montage.AS_BossLongSwing_Montage'"));
    if (montageObj.Succeeded())
    {
        swordNormalAttack02 = montageObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj2(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_NormalAttack03V4_Montage.AM_NormalAttack03V4_Montage'"));
    if (montageObj2.Succeeded())
    {
        swordNormalAttack02 = montageObj2.Object;
    }
    bNotifyTick = true;
}

EBTNodeResult::Type UTask_NormalAttack02::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    TickTask(OwnerComp, NodeMemory, 0.0f);

    if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
    {
        bossController->StopMovement();
        APawn* ControlledPawn = bossController->GetPawn();
        if (ControlledPawn)
        {
            ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);

            if (swordNormalAttack02 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance())
            {
                //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                boss->ServerRPC_NormalAttack02V1();

                if (ABossApernia* bossComponent = Cast<ABossApernia>(OwnerComp.GetAIOwner()->GetPawn()))
                {
                   
                    bossComponent->bossSwordComp->SetRelativeLocation(FVector(17.137708f, 57.508425f, 23.246429f));
                    bossComponent->bossSwordComp->SetRelativeRotation(FRotator(28.852794f, 169.726741f, 183.362852f));
                }
            }
        }
    }

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

    return EBTNodeResult::InProgress;


}

void UTask_NormalAttack02::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // 태스크 시작 시 초기화
    if (currentTime == 0.0f)
    {
        animOnce = false;
    }

    currentTime += DeltaSeconds;

    if (currentTime > 1.8 && currentTime < 1.9)
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





    if (currentTime > 2.0 && currentTime < 2.2)
    {
        

        if (ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetOwner()))
        {
            APawn* ControlledPawn = bossController->GetPawn();
            if (ControlledPawn)
            {
                ABossApernia* boss = Cast<ABossApernia>(ControlledPawn);

                if (swordNormalAttack02 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnceV2)
                {
                    //애니메이션을 실행하되 Delegate로 애니메이션이 끝난후 EBTNodeResult::Succeeded를 리턴
                    UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();

                    boss->ServerRPC_NormalAttack02V2();
                    animOnceV2 = true;

                }
            }
        }
    }




    // 1.8초가 지나면 태스크 완료
    if (currentTime >= 3.5f)
    {
        normalAttack02 = false;
        animOnceV2 = false;
        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
        BlackboardComp = OwnerComp.GetBlackboardComponent();
        if (BlackboardComp)
        {
            BlackboardComp->SetValueAsBool(normalAttack02CoolTime.SelectedKeyName, normalAttack02);
        }
        currentTime = 0.0f; // currentTime 초기화
        if (ABossApernia* bossComponent = Cast<ABossApernia>(OwnerComp.GetAIOwner()->GetPawn()))
        {
            
            if (player)
            {
                bossComponent->bossSwordComp->SetRelativeLocation(FVector(29.425722f, 55.060376f, 8.3646449f));
                bossComponent->bossSwordComp->SetRelativeRotation(FRotator(4.826905f, 1.306981f, 8.324931f));
            }
        }
        

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
       
    }
}
