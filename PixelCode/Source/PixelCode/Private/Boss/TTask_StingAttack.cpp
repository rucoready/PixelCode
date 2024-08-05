// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/TTask_StingAttack.h"
#include "Boss/BossAIController.h"
#include "Boss/BossApernia.h"
#include "Animation/AnimInstance.h"
#include "Player/PixelCodeCharacter.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


UTTask_StingAttack::UTTask_StingAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Sting Attack 01");

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AS_BossLongAttack01_Montage.AS_BossLongAttack01_Montage'"));
    if (montageObj.Succeeded())
    {
        stingAttack01 = montageObj.Object;
    }
    bNotifyTick = true;

    
}

EBTNodeResult::Type UTTask_StingAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    TickTask(OwnerComp, NodeMemory, 0.0f);
    animOnce = false;
    ABossAIController* BossAIController = Cast<ABossAIController>(OwnerComp.GetAIOwner());

    if (BossAIController)
    {
        // BossAIController에 StopMovement() 호출하여 움직임 멈춤
        BossAIController->StopMovement();

        
            
    }

    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

    int32 randomIndex = FMath::RandRange(0, foundCharacters.Num() - 1);
    player = Cast<APixelCodeCharacter>(foundCharacters[randomIndex]);

    return EBTNodeResult::InProgress;

    
}

void UTTask_StingAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    // 태스크 시작 시 초기화
    if (currentTime == 0.0f)
    {
        animOnce = false;
    }

    currentTime += DeltaSeconds;

    
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
                //currentTime이 0.5초보다 작을떄 보
                if (currentTime < 0.5f)
                {
                    // 방향 설정
                    FVector direction = playerLocation - bossPawn->GetActorLocation();
                    direction.Z = 0; // 보스가 수평으로만 회전하도록 Z축 회전 제거
                    FRotator newRotation = direction.Rotation();
                    bossPawn->SetActorRotation(newRotation);
                }
                if (currentTime < 1.5)
                {
                    // 애니메이션 재생
                    ABossApernia* boss = Cast<ABossApernia>(bossPawn);
                    if (stingAttack01 && boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnce)
                    {
                        UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();
                        boss->ServerRPC_StingAttack();
                        animOnce = true;

                        boss->ServerRPC_StingAttackSwordPositionReSet();

                        
                    }
                }

                if (currentTime > 1.5)
                {
                    if (player)
                    {
                        APlayerController* pc = player->GetController<APlayerController>();
                        if (pc != nullptr && !onceCameraShake)
                        {
                            pc->ClientStartCameraShake(cameraShakeOBJ);
                            onceCameraShake = true;


                        }
                    }
                }

                if (currentTime > 1.3 && !onceSpawnStingNiagara)
                {
                    if (ABossApernia* boss = Cast<ABossApernia>(OwnerComp.GetAIOwner()->GetPawn()))
                    {
                        boss->ServerRPC_StingAttackCameraShake();
                        boss->ServerRPC_SpawnNiagaraStingAttack();
                        onceSpawnStingNiagara = true;
                    }

                }
                else if (!animOnce) // currentTime >= 1.5 이후에 한 번만 실행되도록 조건 추가
                {
                    // 방향 설정 후 이후에는 방향을 변경하지 않음
                    animOnce = true;
                }

                // 이동 관련 처리
                if (currentTime >= 1.1 && currentTime <= 1.6) // 1.0~ 1.5 위치계산
                {
                    // 현재 위치와 목표 위치 계산
                    FVector currentLocation = bossPawn->GetActorLocation();
                    FVector targetLocation = currentLocation + bossPawn->GetActorForwardVector() * speed * DeltaSeconds * 4;
     
                    // 실제 이동
                    bossPawn->SetActorLocation(targetLocation);

                    
                }
            }
        }
        
        if (ABossApernia* boss = Cast<ABossApernia>(OwnerComp.GetAIOwner()->GetPawn()))
        {
            if (currentTime > 1.7f)
            {
                boss->ServerRPC_StingAttackSwordPositionReSet();
            }
        }
    }

    // 1.8초가 지나면 태스크 완료
    if (currentTime >= 1.8f)
    {
        ABossAIController* BossAIController = Cast<ABossAIController>(OwnerComp.GetAIOwner());

        
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        animOnce = false;
        currentTime = 0.0f; // currentTime 초기화
        currentSpeed = 0.0f; // 현재 속도 초기화
        bDirectionSet = false;
        onceCameraShake = false; 
        onceSpawnStingNiagara = false;
    }
}