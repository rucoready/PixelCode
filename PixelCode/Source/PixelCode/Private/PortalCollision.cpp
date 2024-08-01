#include "PortalCollision.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "PortalRobbyWidget.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "PCodePlayerController.h"
#include "MyGameModeBase.h"

// Sets default values
APortalCollision::APortalCollision()
{
    // Set this actor to call Tick() every frame.
    PrimaryActorTick.bCanEverTick = true;

    boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("boxCollision"));
}

// Called when the game starts or when spawned
void APortalCollision::BeginPlay()
{
    Super::BeginPlay();
    boxCollision->OnComponentBeginOverlap.AddDynamic(this, &APortalCollision::OnBeginOverlapPortal);
    boxCollision->OnComponentEndOverlap.AddDynamic(this, &APortalCollision::OneEndOverlapPortal);

    bReady = false;
    MyGameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(this));
   
}

// Called every frame
void APortalCollision::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
}

bool APortalCollision::IsServer() const
{
    return GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer;
}

void APortalCollision::OnBeginOverlapPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    
    if (OtherActor && OtherActor->GetName().Contains("Player"))
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Overlap Check"));
        if (portalRobbyWidget)
        {
            APawn* OverlappedPawn = Cast<APawn>(OtherActor);
            if (OverlappedPawn)
            {
                // APCodePlayerController 타입으로 캐스팅
                APCodePlayerController* OverlappedPlayerController = Cast<APCodePlayerController>(OverlappedPawn->GetController());
                if (OverlappedPlayerController)
                {
                    OverlappedPlayerController->ServerRPC_CreateWidgetRobbyWidget();

                }
            }
        }

    }
}

void APortalCollision::OneEndOverlapPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor->GetName().Contains("Player"))
    {
        ServerRPC_HideRobbyWidget();
    }
}

void APortalCollision::ServerTravel()
{
    

    
}

void APortalCollision::OnRep_Ready()
{
    if (WidgetInstance)
    {
        if (bIsReady)
        {
            WidgetInstance->SetText();
        }
        else
        {
            WidgetInstance->NoSetText();
        }
        WidgetInstance->OnMyclickButtonPlayer1();
    }

}

void APortalCollision::ChangeBisReady(bool newReady)
{
    bIsReady = newReady;
    OnRep_Ready();
}

void APortalCollision::ServerRPC_HideRobbyWidget_Implementation()
{
    MulticastRPC_HideRobbyWidget();
}

void APortalCollision::MulticastRPC_HideRobbyWidget_Implementation()
{
    if (portalRobbyWidget)
    {
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            // PCodePlayerController 타입으로 캐스팅
            APCodePlayerController* PCodePlayerController = Cast<APCodePlayerController>(It->Get());
            if (PCodePlayerController)
            {
                PCodePlayerController->ServerRPC_HideWidgetRobbyWidget();
 

            }
        }
    }
    

}

void APortalCollision::ServerRPC_ShowRobbyWidget_Implementation()
{

}

void APortalCollision::MulticastRPC_ShowRobbyWidget_Implementation()
{

}

void APortalCollision::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(APortalCollision, bIsReady);

}
