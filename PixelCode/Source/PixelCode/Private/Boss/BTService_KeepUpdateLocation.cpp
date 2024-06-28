// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BTService_KeepUpdateLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Player/PixelCodeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

UBTService_KeepUpdateLocation::UBTService_KeepUpdateLocation()
{
	NodeName = TEXT("Service Find Player Location");
	//UE_LOG(LogTemp, Warning, TEXT("Location Update Service Check"));
	Interval = 1.0f;
}

void UBTService_KeepUpdateLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	//APixelCodeCharacter* PlayerCharacter = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	APixelCodeCharacter* PlayerCharacter = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerCharacter)
	{
		return;
	}

	FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(PlayerLocationKey.SelectedKeyName, PlayerLocation);

	//if (PlayerCharacter == nullptr) return;
	//FVector PlayerLocation;
	

	//if (auto* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("GetPlayerCharacter!!"));
	//	auto playerLocation = player->GetActorLocation();
		
	//	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
		
	//}
}
