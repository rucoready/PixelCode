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
#include "GruxAIController.h"
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

   

        gruxLoc = grux->GetActorLocation();
        FVector gruxForward = grux->GetActorForwardVector();

        TArray<AActor*> foundCharacters;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);
        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

        const int32 numTraces = 40;
        const float angleStep = 160.0f / numTraces;
        const float halfFOV = 160.0f / 2.0f;

        bAnyPlayerDetected = false;

        for (AActor* actor : foundCharacters)
        {
            actorLoc = actor->GetActorLocation();
            FVector toActor = (actorLoc - gruxLoc).GetSafeNormal();
            distance = FVector::Dist(gruxLoc, actorLoc);
            float dotProduct = FVector::DotProduct(gruxForward, toActor);
            float angle = FMath::Acos(dotProduct) * (180.0f / PI);

            if (angle <= 80.0f)
            {
                for (int32 i = 0; i <= numTraces; ++i)
                {
                    float currentAngle = -halfFOV + i * angleStep;
                    FRotator rotation = FRotator(0.0f, currentAngle, 0.0f);
                    FVector direction = rotation.RotateVector(gruxForward);

                    FVector traceEnd = gruxLoc + direction * 1500.0f;
                    FHitResult hitResult;
                    FCollisionQueryParams queryParams;
                    queryParams.AddIgnoredActor(grux);

                    bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, gruxLoc, traceEnd, ECC_Visibility, queryParams);

                    //DrawDebugLine(GetWorld(), gruxLoc, traceEnd, FColor::Red, false, 0.1f, 0, 10.0f);

                    if (bHit && hitResult.GetActor())
                    {
                        FString actorName = hitResult.GetActor()->GetName();
                        if (actorName.Contains("Player"))
                        {
                            player = Cast<APixelCodeCharacter>(hitResult.GetActor());
                            
                            bAnyPlayerDetected = true;


                        }

                    }
                }

                if (bAnyPlayerDetected)
                {
                    break;
                }
            }
        }

        if (!bAnyPlayerDetected)
        {
            UE_LOG(LogTemp, Warning, TEXT("NoDetected"));
        }

        bPlayerInSight = bAnyPlayerDetected;
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bPlayerInSight);

        if (bPlayerInSight)
        {
            for (AActor* actor : foundCharacters)
            {
                actorLoc = actor->GetActorLocation();
                distance = FVector::Dist(gruxLoc, actorLoc);

                if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
                {
                    AGruxAIController* gruxController = Cast<AGruxAIController>(OwnerComp.GetAIOwner());
                    if (gruxController)
                    {
                        if (distance < 300)
                        {
                            gruxController->StopMovement();
                            bGuxCanAttack = true;

                            if (!bOnceTimer)
                            {
                                attackWaitTime = currentTime;
                                bOnceTimer = true;
                            }

                            BlackboardComp->SetValueAsBool(canAttackKey.SelectedKeyName, bGuxCanAttack);
                        }
                        else
                        {
                            gruxController->MoveToLocation(actorLoc);
                            bGuxCanAttack = false;
                            BlackboardComp->SetValueAsBool(canAttackKey.SelectedKeyName, bGuxCanAttack);
                            bOnceTimer = false;
                        }
                    }
                }
            }
        }

        if (bGuxCanAttack)
        {
            if (currentTime - attackWaitTime > 2.0f)
            {
                UE_LOG(LogTemp, Warning, TEXT("BBBBBBBBBBBBBBBBBBBBBB"));
                if (!bCheckingBehind) // CheckBehind 상태가 아니면
                {
                    checkBehindStartTime = currentTime; // CheckBehind 시작 시간 설정
                    bCheckingBehind = true;
                }
            }
        }

        if (bCheckingBehind)
        {
            if (currentTime - checkBehindStartTime < 2.0f) // 2초 동안 CheckBehind 유지
            {
                CheckBehindAfterAttack(OwnerComp.GetBlackboardComponent());
            }
            else
            {
                bCheckingBehind = false; // 2초가 지나면 CheckBehind 상태 종료
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

        //DrawDebugLine(GetWorld(), gruxLoc, traceEnd, FColor::Blue, false, 0.1f, 0, 2.0f);

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
