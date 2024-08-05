// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_GruxCheckAttack.h"
#include "Kismet/GameplayStatics.h"
#include "GruxAIController.h"
#include "Grux.h"
#include "Player/PixelCodeCharacter.h" //캐릭터 임시
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UService_GruxCheckAttack::UService_GruxCheckAttack()
{
	NodeName = TEXT("Check Can Attack");
}

void UService_GruxCheckAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    //그럭스 찾기
    AGrux* grux = Cast<AGrux>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrux::StaticClass()));
    FVector gruxLoc = grux->GetActorLocation();
    FVector gruxForward = grux->GetActorForwardVector();
    
    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);
    float betweenSize = FVector::Dist2D(gruxLoc, actorLoc);

    

    for (AActor* actor : foundCharacters)
    {
        actorLoc = actor->GetActorLocation();

        if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
        {
            AGruxAIController* gruxController = Cast<AGruxAIController>(OwnerComp.GetAIOwner());
            if (gruxController)
            {
                

                if (betweenSize > stopDistance)
                {
                    canAttack = false;
                    OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), canAttack);
                }
                else
                {
                    canAttack = true;
                    OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), canAttack);

                }
            }
        }
    }

}
