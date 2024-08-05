// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_DoubleSwing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Boss/BossAIController.h"
#include "Player/PixelCodeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossApernia.h"
#include "Camera/CameraShakeBase.h"
#include "AIController.h" // 임시
#include "Particles/ParticleSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"

UTask_DoubleSwing::UTask_DoubleSwing(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Double Swing Attack");

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AM_DoubleSwing_Montage.AM_DoubleSwing_Montage'"));
    if (montageObj.Succeeded())
    {
        doubleSwingAttack = montageObj.Object;
    }
    bNotifyTick = true;
    
}

EBTNodeResult::Type UTask_DoubleSwing::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 태스크 시작 시 필요한 변수 초기화
    currentTime = 0.0f;
    firstSwingAttack = false;
    changeSwordPosition1 = false;
    
    animOnce = false;
    animOnce2 = false;
    onceSpawnStingNiagara = false;
    onceSpawnStingNiagara2 = false;

    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

    int32 randomIndex = FMath::RandRange(0, foundCharacters.Num() - 1);
    player = Cast<APixelCodeCharacter>(foundCharacters[randomIndex]);

    // 애니메이션 재생 시작
    TickTask(OwnerComp, NodeMemory, 0.0f);

    return EBTNodeResult::InProgress;
}



void UTask_DoubleSwing::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    currentTime += DeltaSeconds;

   
    if (player)
    {
        // 플레이어의 위치를 얻어낸다
        playerLocation = player->GetActorLocation();
        // 보스컨트롤러를 캐스팅
        ABossAIController* bossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
        if (bossController)
        {
            bossController->StopMovement();
            APawn* bossPawn = bossController->GetPawn();
            if (bossPawn)
            {
                if (currentTime == 0)
                {
                    // 애니메이션 재생
                    ABossApernia* boss = Cast<ABossApernia>(bossPawn);
                    if (boss->GetMesh() && !changeSwordPosition1)
                    {
                        
                        boss->bossSwordComp->SetRelativeLocation(FVector(27.264644f, 55.392292f, -3.764748f));
                        boss->bossSwordComp->SetRelativeRotation(FRotator(1.849770f, 172.506280f, 197.859334f));
                        
                        changeSwordPosition1 = true;

                    }
                }
                if (currentTime == 1.65)
                {
                    // 애니메이션 재생
                    ABossApernia* boss = Cast<ABossApernia>(bossPawn);
                    if (boss->GetMesh() && !changeSwordPosition1)
                    {
                        boss->bossSwordComp->SetRelativeLocation(FVector(19.656581, 72.256598, -31.534782));
                        boss->bossSwordComp->SetRelativeRotation(FRotator(-0.517038f, 180.644218f, 202.044021));
                        changeSwordPosition1 = true;

                    }
                }
                
                if (currentTime < 0.5f)
                {
                    // 방향 설정
                    FVector direction = playerLocation - bossPawn->GetActorLocation();
                    direction.Z = 0; // 보스가 수평으로만 회전하도록 Z축 회전 제거
                    FRotator newRotation = direction.Rotation();
                    bossPawn->SetActorRotation(newRotation);
                }
                if (currentTime < 1.5f)
                {
                    // 애니메이션 재생
                    ABossApernia* boss = Cast<ABossApernia>(bossPawn);
                    if (doubleSwingAttack && boss->GetMesh() && boss->GetMesh()->GetAnimInstance() && !animOnce)
                    {
                        UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();
                        boss->ServerRPC_DoubleSwingAttack();
                        animOnce = true;

                    }
                }
                else if (!animOnce) // currentTime >= 1.5 이후에 한 번만 실행되도록 조건 추가
                {
                    // 방향 설정 후 이후에는 방향을 변경하지 않음
                    animOnce = true;
                }

                // 이동 관련 처리
                if (currentTime >= 0.3f && currentTime <= 0.5f) // 0.2~0.5초 사이에 최대 200만큼 이동
                {
                    // 현재 위치와 목표 위치 계산
                    FVector currentLocation = bossPawn->GetActorLocation();
                    FVector targetLocation = currentLocation + bossPawn->GetActorForwardVector() * speed * DeltaSeconds * 4;

                    // 최대 이동 거리 200 제한
                    FVector movement = targetLocation - currentLocation;
                    if (movement.Size() > 200.0f)
                    {
                        movement = movement.GetSafeNormal() * 200.0f;
                        targetLocation = currentLocation + movement;
                    }

                    
                    // 실제 이동
                    bossPawn->SetActorLocation(targetLocation);
                }
                // 이동 관련 처리
                if (currentTime >= 0.5f && currentTime <= 0.7f) // 0.5~1.0초 사이에 최대 700만큼 이동
                {
                    // 현재 위치와 목표 위치 계산
                    FVector currentLocation = bossPawn->GetActorLocation();
                    FVector targetLocation = currentLocation + bossPawn->GetActorForwardVector() * speed2 * DeltaSeconds * 4;

                    // 최대 이동 거리 700 제한
                    FVector movement = targetLocation - currentLocation;
                    if (movement.Size() > 400.0f)
                    {
                        movement = movement.GetSafeNormal() * 700.0f;
                        targetLocation = currentLocation + movement;
                    }

                    
                    // 실제 이동
                    bossPawn->SetActorLocation(targetLocation);
                }

                // 나이아가라 파티클 스폰 (첫 번째 조건)
                if (currentTime > 0.55 && !onceSpawnStingNiagara)
                {
                    if (ABossApernia* boss = Cast<ABossApernia>(OwnerComp.GetAIOwner()->GetPawn()))
                    {
                        boss->ServerRPC_SpawnNiagaraDoubleSwing01();

                        onceSpawnStingNiagara = true;

                        APlayerController* pc = player->GetController<APlayerController>();
                        if (pc != nullptr)
                        {
                            pc->ClientStartCameraShake(cameraShakeOBJ);
                            boss->ServerRPC_DoubleSwingCameraShake();
                        }
                    }
                }

                // 나이아가라 파티클 스폰 (두 번째 조건)
                if (currentTime > 1.4 && !onceSpawnStingNiagara2)
                {
                    if (ABossApernia* boss = Cast<ABossApernia>(OwnerComp.GetAIOwner()->GetPawn()))
                    {
                        boss->ServerRPC_SpawnNiagaraDoubleSwing02();

                        onceSpawnStingNiagara2 = true;

                        APlayerController* pc = player->GetController<APlayerController>();
                        if (pc != nullptr)
                        {
                            pc->ClientStartCameraShake(cameraShakeOBJ);
                            boss->ServerRPC_DoubleSwingCameraShake();
                        }
                    }
                }


                ///////////////////////2
                if (currentTime < 1.0f)
                {
                    // 방향 설정
                    FVector direction = playerLocation - bossPawn->GetActorLocation();
                    direction.Z = 0; // 보스가 수평으로만 회전하도록 Z축 회전 제거
                    FRotator newRotation = direction.Rotation();
                    bossPawn->SetActorRotation(newRotation);
                }
                if (currentTime > 1.0f)
                {
                    // 애니메이션 재생
                    ABossApernia* boss = Cast<ABossApernia>(bossPawn);
                    if (doubleSwingAttack && boss->GetMesh()&&boss->GetMesh()->GetAnimInstance() && !animOnce2)
                    {
                        UAnimInstance* AnimInstance = boss->GetMesh()->GetAnimInstance();
                        boss->ServerRPC_DoubleSwingAttack();
                        animOnce2 = true;

                       
                    }
                }
                else if (!animOnce) // currentTime >= 1.5 이후에 한 번만 실행되도록 조건 추가
                {
                    // 방향 설정 후 이후에는 방향을 변경하지 않음
                    animOnce2 = true;
                }

                // 이동 관련 처리
                if (currentTime >= 1.0f && currentTime <= 1.2f) // 0.2~0.5초 사이에 최대 200만큼 이동
                {
                    // 현재 위치와 목표 위치 계산
                    FVector currentLocation = bossPawn->GetActorLocation();
                    FVector targetLocation = currentLocation + bossPawn->GetActorForwardVector() * speed * DeltaSeconds * 4;

                    // 최대 이동 거리 200 제한
                    FVector movement = targetLocation - currentLocation;
                    if (movement.Size() > 200.0f)
                    {
                        movement = movement.GetSafeNormal() * 200.0f;
                        targetLocation = currentLocation + movement;
                    }

                    
                    // 실제 이동
                    bossPawn->SetActorLocation(targetLocation);
                }
                // 이동 관련 처리
                if (currentTime >= 1.2f && currentTime <= 1.4f) // 0.5~1.0초 사이에 최대 700만큼 이동
                {
                    // 현재 위치와 목표 위치 계산
                    FVector currentLocation = bossPawn->GetActorLocation();
                    FVector targetLocation = currentLocation + bossPawn->GetActorForwardVector() * speed2 * DeltaSeconds * 4;

                    // 최대 이동 거리 700 제한
                    FVector movement = targetLocation - currentLocation;
                    if (movement.Size() > 400.0f)
                    {
                        movement = movement.GetSafeNormal() * 700.0f;
                        targetLocation = currentLocation + movement;
                    }

                    
                    // 실제 이동
                    bossPawn->SetActorLocation(targetLocation);
                }
            }
        }
    }

    // 1.8초가 지나면 태스크 완료
    if (currentTime >= 1.8f)
    {
        doubleSwingAttack1 = false;
        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
        BlackboardComp = OwnerComp.GetBlackboardComponent();
        if (BlackboardComp)
        {
            BlackboardComp->SetValueAsBool(doubleSwingAttackCoolTime.SelectedKeyName, doubleSwingAttack1);
        }

        
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        
    }
}
