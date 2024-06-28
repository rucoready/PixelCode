// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossAIController.h"
#include "Boss/TestBoss.h"
#include "Boss/BossApernia.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"




ABossAIController::ABossAIController(FObjectInitializer const& ObjectInitializer)
    : Super(ObjectInitializer)
{
    
	//SetupPerceptionSystem();
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
    
	if (ABossApernia* const bossApernia = Cast<ABossApernia>(InPawn))
	{
		if (UBehaviorTree* const tree = bossApernia->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);

		}
	}
}



//void ABossAIController::SetupPerceptionSystem()
//{
    // �ν� ������Ʈ�� ���� �⺻ ���� ������Ʈ ���� �� ����
   // SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
    //sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("UAISenseConfig_Sight"));
   // if (sightConfig)
   // {
        
        
     //   sightConfig->SightRadius = 1500.0f;
        
    //    sightConfig->LoseSightRadius = sightConfig->SightRadius + 25.0f;
        
    //    sightConfig->PeripheralVisionAngleDegrees = 150.0f;
        
     //   sightConfig->SetMaxAge(5.f);
        
     //   sightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f;
        
    //    sightConfig->DetectionByAffiliation.bDetectEnemies = true;
     //   sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
     //   sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
        
     //   GetPerceptionComponent()->SetDominantSense(*sightConfig->GetSenseImplementation());
        
    //    GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABossAIController::OnTargetDetected);
        
    //    GetPerceptionComponent()->ConfigureSense((*sightConfig));
    //}
    
//}

//void ABossAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
//{
    
    

    //if (Actor && Actor->GetActorNameOrLabel().Contains("ThirdPerson"))
    //{
        //UE_LOG(LogTemp, Warning, TEXT("Successfully cast to AThirdPersonCharacter"));
     //   GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
        //UE_LOG(LogTemp, Warning, TEXT("Can See Player!!"));
    //}
    //else
    //{
        //UE_LOG(LogTemp, Warning, TEXT("Failed to cast to AThirdPersonCharacter"));

    //}
    //if (ATestBoss* const TestBoss = Cast<ATestBoss>(Actor))
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("Successfully cast to ATestBoss"));
    //    GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("Failed to cast to ATestBoss"));
    //}
    
//}
