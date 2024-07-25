// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_DogSense.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Player/PixelCodeCharacter.h"
#include "DogBart.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DogBartAIController.h"

UService_DogSense::UService_DogSense()
{
    NodeName = TEXT("Player Sense Check");
    currentTime = 0.0f;
}

void UService_DogSense::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    currentTime += DeltaSeconds;

    TArray<AActor*> foundDogBart;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADogBart::StaticClass(), foundDogBart);

    dogBart = Cast<ADogBart>(OwnerComp.GetAIOwner()->GetPawn());

    dogLoc = dogBart->GetActorLocation();
    FVector gruxForward = dogBart->GetActorForwardVector();

    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    const int32 numTraces = 40;
    const float angleStep = 160.0f / numTraces;
    const float halfFOV = 160.0f / 2.0f;

    bAnyPlayerDetected = false;
    FString closestPlayerName;
    FVector closestPlayerLoc = FVector::ZeroVector; // 초기값 설정
    float closestDistance = FLT_MAX;

    for (AActor* actor : foundCharacters)
    {
        actorLoc = actor->GetActorLocation();
        FVector toActor = (actorLoc - dogLoc).GetSafeNormal();
        distance = FVector::Dist(dogLoc, actorLoc);
        float dotProduct = FVector::DotProduct(gruxForward, toActor);
        float angle = FMath::Acos(dotProduct) * (180.0f / PI);

        if (angle <= 80.0f && distance <= 1700.0f)
        {
            bool bPlayerDetected = false;

            for (int32 i = 0; i <= numTraces; ++i)
            {
                float currentAngle = -halfFOV + i * angleStep;
                FRotator rotation = FRotator(0.0f, currentAngle, 0.0f);
                FVector direction = rotation.RotateVector(gruxForward);

                FVector traceStart = dogLoc;
                FVector traceEnd = traceStart + direction * 1700.0f;

                TArray<FHitResult> hitResults; // FHitResult 배열
                FCollisionQueryParams queryParams;
                queryParams.AddIgnoredActor(dogBart);

                float attackRadius = 700.0f;
                bool bHit = GetWorld()->SweepMultiByChannel(hitResults, traceStart, traceEnd, FQuat::Identity,
                    ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(attackRadius), queryParams);

                // DrawDebugSphere(GetWorld(), traceEnd, attackRadius, 12, FColor::Red, false, 0.1f, 0, 10.0f);

                // hitResult는 hitResults 배열 안의 결과들을 반복하면서 접근
                for (const FHitResult& hitResult : hitResults)
                {
                    AActor* hitActor = hitResult.GetActor();
                    if (hitActor)
                    {
                        FString actorName = hitActor->GetName();
                        if (actorName.Contains("Player"))
                        {
                            float hitDistance = FVector::Dist(dogLoc, hitActor->GetActorLocation());

                            if (hitDistance < closestDistance)
                            {
                                closestDistance = hitDistance;
                                closestPlayerName = actorName;
                                closestPlayerLoc = hitActor->GetActorLocation(); // 가장 가까운 플레이어의 위치 저장
                            }

                            bPlayerDetected = true;
                        }
                    }
                }

                if (bPlayerDetected)
                {
                    bAnyPlayerDetected = true;
                    break; // 더 이상 검사할 필요 없음
                }
            }
        }
    }

    // 블랙보드에 값 설정
    if (bAnyPlayerDetected)
    {
        BlackboardComp->SetValueAsBool(playerInSight.SelectedKeyName, bAnyPlayerDetected);
        BlackboardComp->SetValueAsString(detectedPlayerName.SelectedKeyName, closestPlayerName);
        BlackboardComp->SetValueAsVector(detectedPlayerLoc.SelectedKeyName, closestPlayerLoc); // 가장 가까운 플레이어의 위치를 블랙보드에 설정
    }
    else
    {
        BlackboardComp->SetValueAsBool(playerInSight.SelectedKeyName, bAnyPlayerDetected);
        BlackboardComp->SetValueAsString(detectedPlayerName.SelectedKeyName, TEXT(""));  // 이름을 빈 문자열로 설정
        BlackboardComp->SetValueAsVector(detectedPlayerLoc.SelectedKeyName, FVector::ZeroVector); // 위치를 초기값으로 설정
        dogBart->GetCharacterMovement()->MaxWalkSpeed = 400.0f;
    }
}
