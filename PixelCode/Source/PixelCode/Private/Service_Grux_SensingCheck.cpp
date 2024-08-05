// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_Grux_SensingCheck.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Player/PixelCodeCharacter.h"
#include "Grux.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Grux.h"
#include "DrawDebugHelpers.h"
#include "Math/RotationMatrix.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GruxAIController.h"

UService_Grux_SensingCheck::UService_Grux_SensingCheck()
{
    NodeName = TEXT("Player Sense Check");
    currentTime = 0.0f;
}

void UService_Grux_SensingCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

    currentTime += DeltaSeconds;

    TArray<AActor*> foundGrux;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrux::StaticClass(), foundGrux);

    grux = Cast<AGrux>(OwnerComp.GetAIOwner()->GetPawn());
    if (!grux) return; // grux가 유효하지 않으면 함수 종료

    gruxLoc = grux->GetActorLocation();
    FVector gruxForward = grux->GetActorForwardVector();

    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    const int32 numTraces = 40;
    const float angleStep = 160.0f / numTraces;
    const float halfFOV = 160.0f / 2.0f;

    bAnyPlayerDetected = false;
    APixelCodeCharacter* closestPlayer = nullptr;
    float closestDistance = FLT_MAX;

    for (int32 i = 0; i <= numTraces; ++i)
    {
        float currentAngle = -halfFOV + i * angleStep;
        FRotator rotation = FRotator(0.0f, currentAngle, 0.0f);
        FVector direction = rotation.RotateVector(gruxForward);

        FVector traceEnd = gruxLoc + direction * 1500.0f;
        TArray<FHitResult> hitResults; // FHitResult 배열
        FCollisionQueryParams queryParams;
        queryParams.AddIgnoredActor(grux);

        float attackRadius = 700.0f;
        bool bHit = GetWorld()->SweepMultiByChannel(hitResults, gruxLoc, traceEnd, FQuat::Identity,
            ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(attackRadius), queryParams);

        // DrawDebugSphere로 디버그 시각화
        //DrawDebugSphere(GetWorld(), traceEnd, attackRadius, 12, FColor::Red, false, 0.1f, 0, 10.0f);

        for (const FHitResult& hitResult : hitResults)
        {
            AActor* hitActor = hitResult.GetActor();
            if (hitActor)
            {
                FString actorName = hitActor->GetName();
                if (actorName.Contains("Player"))
                {
                    APixelCodeCharacter* detectedPlayer = Cast<APixelCodeCharacter>(hitActor);
                    if (detectedPlayer)
                    {
                        FVector detectedPlayerLoc = detectedPlayer->GetActorLocation();
                        distance = FVector::Dist2D(gruxLoc, detectedPlayerLoc); // Z축 무시

                        if (distance < closestDistance)
                        {
                            closestPlayer = detectedPlayer;
                            closestDistance = distance;
                            bAnyPlayerDetected = true;
                            
                        }
                    }
                }
            }
        }
    }

    if (closestPlayer)
    {
        bPlayerInSight = true;
        FVector playerLoc = closestPlayer->GetActorLocation();
        FVector directionToPlayer = (playerLoc - gruxLoc).GetSafeNormal2D();
        FRotator targetRotation = directionToPlayer.Rotation();

        if (AGruxAIController* gruxController = Cast<AGruxAIController>(OwnerComp.GetAIOwner()))
        {
            // 부드러운 회전 적용
            FRotator currentRotation = grux->GetActorRotation();
            FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, DeltaSeconds, 5.0f); // 속도 조절

            grux->SetActorRotation(newRotation);
            BlackboardComp->SetValueAsVector(nearlestPlayerLocation.SelectedKeyName, playerLoc);
            

            // 거리 기반의 행동 결정
            if (closestDistance < 250.0f)
            {
                // 플레이어와의 거리가 250 이하일 때
                gruxController->StopMovement();
                bGuxCanAttack = true;

                // 일정 시간 동안 이 거리 내에 있을 때만 공격 허용
                if (!bOnceTimer)
                {
                    attackWaitTime = currentTime;
                    bOnceTimer = true;
                }
                BlackboardComp->SetValueAsBool(canAttackKey.SelectedKeyName, bGuxCanAttack);
            }
            else if (closestDistance < 300.0f)
            {
                // 거리 250과 300 사이에서는 느리게 이동
                if (currentTime - attackWaitTime > 1.0f) // 일정 시간 이상 거리가 유지되면 이동 시작
                {
                    gruxController->MoveToLocation(playerLoc);
                    bGuxCanAttack = false;
                    BlackboardComp->SetValueAsBool(canAttackKey.SelectedKeyName, bGuxCanAttack);
                    bOnceTimer = false;
                }
            }
            else
            {
                // 거리가 300 이상일 때는 계속 이동
                gruxController->MoveToLocation(playerLoc);
                bGuxCanAttack = false;
                BlackboardComp->SetValueAsBool(canAttackKey.SelectedKeyName, bGuxCanAttack);
                bOnceTimer = false;
            }
        }
    }
    else
    {
        bPlayerInSight = false;
       
    }

    OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bPlayerInSight);

    if (bGuxCanAttack)
    {
        if (currentTime - attackWaitTime > 2.0f)
        {
            if (!bCheckingBehind)
            {
                checkBehindStartTime = currentTime;
                bCheckingBehind = true;
            }
        }
    }

    if (bCheckingBehind)
    {
        if (currentTime - checkBehindStartTime < 2.0f)
        {
            CheckBehindAfterAttack(OwnerComp.GetBlackboardComponent());
        }
        else
        {
            bCheckingBehind = false;
        }
    }
}

void UService_Grux_SensingCheck::CheckBehindAfterAttack(UBlackboardComponent* BlackboardComp)
{
    for (int32 i = 0; i < 360; i += 10)
    {
        FRotator rotation = FRotator(0.0f, i, 0.0f);
        FVector direction = rotation.RotateVector(FVector::ForwardVector);

        FVector traceEnd = gruxLoc + direction * 900.0f;
        FHitResult hitResult;
        FCollisionQueryParams queryParams;
        queryParams.AddIgnoredActor(grux);

        bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, gruxLoc, traceEnd, ECC_Visibility, queryParams);

        // DrawDebugLine(GetWorld(), gruxLoc, traceEnd, FColor::Blue, false, 0.1f, 0, 2.0f);

        if (bHit && hitResult.GetActor())
        {
            FString actorName = hitResult.GetActor()->GetName();
            if (actorName.Contains("Player"))
            {
                bAnyPlayerDetected = true;
                bGuxCanAttack = true;
                bPlayerInSight = true;
                BlackboardComp->SetValueAsBool(canAttackKey.SelectedKeyName, bGuxCanAttack);
                BlackboardComp->SetValueAsBool(GetSelectedBlackboardKey(), bPlayerInSight);

                AGrux* gruxActor = Cast<AGrux>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrux::StaticClass()));
                if (gruxActor)
                {
                    FRotator currentRotation = gruxActor->GetActorRotation();
                    FRotator targetRotation = direction.Rotation();
                    float rotationSpeed = 5.0f; // 회전 속도 조절
                    FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, GetWorld()->GetDeltaSeconds(), rotationSpeed);

                    gruxActor->SetActorRotation(newRotation);
                }
                break;
            }
        }
    }
}
