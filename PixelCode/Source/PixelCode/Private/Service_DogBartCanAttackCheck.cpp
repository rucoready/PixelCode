// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_DogBartCanAttackCheck.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DogBart.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Player/PixelCodeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DogBartAIController.h"

UService_DogBartCanAttackCheck::UService_DogBartCanAttackCheck()
{
    NodeName = TEXT("DogBart CanAttackCheck");
    currentTime = 0.0f;
}

void UService_DogBartCanAttackCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    currentTime += DeltaSeconds;

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if (BlackboardComp)
    {
        

        // 블랙보드에서 벡터 값을 가져옵니다.
        NearestPlayerLocation = BlackboardComp->GetValueAsVector(TEXT("DetectedPlayerLoc"));

      //  UE_LOG(LogTemp, Warning, TEXT("NearestPlayerLocation: %f, %f, %f"), NearestPlayerLocation.X, NearestPlayerLocation.Y, NearestPlayerLocation.Z);

    }

    dogBart = Cast<ADogBart>(OwnerComp.GetAIOwner()->GetPawn());

    dogBartLoc = dogBart->GetActorLocation();

    distance = FVector::Dist(dogBartLoc, NearestPlayerLocation);

    if (distance < 320)
    {
        meleeAttack = true;
        jumpAttack = false;
        moveNow = false;
        BlackboardComp->SetValueAsBool(jumpAttackOn.SelectedKeyName, jumpAttack);
        BlackboardComp->SetValueAsBool(meleeAttackOn.SelectedKeyName, meleeAttack);
        BlackboardComp->SetValueAsBool(moveOn.SelectedKeyName, moveNow);
        dogBart->GetCharacterMovement()->MaxWalkSpeed = 800.0f;





        
    }
    else if(distance > 321 && distance < 500)
    {
        jumpAttack = true;
        meleeAttack = false;
        moveNow = false;
        BlackboardComp->SetValueAsBool(jumpAttackOn.SelectedKeyName, jumpAttack);
        BlackboardComp->SetValueAsBool(meleeAttackOn.SelectedKeyName, meleeAttack);
        BlackboardComp->SetValueAsBool(moveOn.SelectedKeyName, moveNow);
        dogBart->GetCharacterMovement()->MaxWalkSpeed = 800.0f;
    }
    else
    {
        moveNow = true;
        jumpAttack = false;
        meleeAttack = false;
        BlackboardComp->SetValueAsBool(jumpAttackOn.SelectedKeyName, jumpAttack);
        BlackboardComp->SetValueAsBool(meleeAttackOn.SelectedKeyName, meleeAttack);
        BlackboardComp->SetValueAsBool(moveOn.SelectedKeyName, moveNow);
        dogBart->GetCharacterMovement()->MaxWalkSpeed = 800.0f;
        
        dogBart->ServerRPC_GrowlSound();
           

            
        
        
    }
    
    
}
