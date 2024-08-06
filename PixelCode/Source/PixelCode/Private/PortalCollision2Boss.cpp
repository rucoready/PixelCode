// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalCollision2Boss.h"
#include "PCodePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
APortalCollision2Boss::APortalCollision2Boss()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("boxCollision"));

	RootComponent = boxCollision;

}

// Called when the game starts or when spawned
void APortalCollision2Boss::BeginPlay()
{
	Super::BeginPlay();
	
	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &APortalCollision2Boss::OnBeginOverlapPortal);

	
}

// Called every frame
void APortalCollision2Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortalCollision2Boss::OnBeginOverlapPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		APawn* OverlappedPawn = Cast<APawn>(OtherActor);
		if (OverlappedPawn)
		{
			// APCodePlayerController 타입으로 캐스팅
			APCodePlayerController* OverlappedPlayerController = Cast<APCodePlayerController>(OverlappedPawn->GetController());
			if (OverlappedPlayerController)
			{
				OverlappedPlayerController->CreateWidgetBossEnterWidget();

				UGameplayStatics::PlaySoundAtLocation(GetWorld(), portalCreateSound, GetActorLocation());
			}
		}
	}
	

	
}

void APortalCollision2Boss::RemoveBossLoadingUI()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		// PCodePlayerController 타입으로 캐스팅
		APCodePlayerController* PCodePlayerController = Cast<APCodePlayerController>(It->Get());
		if (PCodePlayerController)
		{
			PCodePlayerController->ServerRPC_HideWidgetBossLoading();
		}
	}
}

void APortalCollision2Boss::ServerRPC_LevelMoveBoss_Implementation()
{
	MulticastRPC_LevelMoveBoss();
	GetWorld()->ServerTravel(TEXT("/Game/KMS_AI/BossMap/BossMap2?Listen"));
	
}

void APortalCollision2Boss::MulticastRPC_LevelMoveBoss_Implementation()
{

	
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		// PCodePlayerController 타입으로 캐스팅
		APCodePlayerController* PCodePlayerController = Cast<APCodePlayerController>(It->Get());
		if (PCodePlayerController)
		{
			PCodePlayerController->ServerRPC_CreateWidgetBossLoading();
			PCodePlayerController->ClientTravel("/Game/KMS_AI/BossMap/BossMap2?Listen", TRAVEL_Absolute);
			//tWorld()->GetTimerManager().SetTimer(handle, this, &APCodePlayerController::ServerRPC_RespawnPlayer_Implementation, 5, false);
		}
	}
}

