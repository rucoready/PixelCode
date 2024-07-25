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
    
// 	if (WidgetInstance)
// 	{
//         UE_LOG(LogTemp, Warning, TEXT("9988"));
//         if (WidgetInstance->ChangeReady1Test == true)
//         {
// 
//             MyGameMode->bIsReadyToReady = true;
//             
//         }
//     }
    
    
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
        ServerRPC_ShowRobbyWidget();
    }
}

void APortalCollision::OneEndOverlapPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor->GetName().Contains("Player"))
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Overlap Disable"));
        ServerRPC_HideRobbyWidget();
    }
}

void APortalCollision::ServerTravel()
{
    
        

        // 서버 트래블 호출
       
        //GetWorld()->ServerTravel(TEXT("/Game/KMS_AI/BossMap/Dungeon2?listen"));
    
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
    

//     for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
//     {
//         APlayerController* BaseController = It->Get();
//         APCodePlayerController* PlayerController = Cast<APCodePlayerController>(BaseController);
//         if (PlayerController && WidgetInstance)
//         {
//             WidgetInstance->RemoveFromParent();
//             PlayerController->bShowMouseCursor = false;
//             PlayerController->bEnableClickEvents = false;
//             PlayerController->bEnableTouchEvents = false;
//         }
//     }
}

void APortalCollision::ServerRPC_ShowRobbyWidget_Implementation()
{
    MulticastRPC_ShowRobbyWidget();
}

void APortalCollision::MulticastRPC_ShowRobbyWidget_Implementation()
{
    if (portalRobbyWidget)
    {
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            // PCodePlayerController 타입으로 캐스팅
            APCodePlayerController* PCodePlayerController = Cast<APCodePlayerController>(It->Get());
            if (PCodePlayerController)
            {
                PCodePlayerController->ServerRPC_CreateWidgetRobbyWidget();
 
                 
            }
        }
    }
}

void APortalCollision::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(APortalCollision, bIsReady);
    /*DOREPLIFETIME(APortalCollision, bIsReadyTextPlayer2);*/
}
