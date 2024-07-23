#include "PortalCollision.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "PortalRobbyWidget.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
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
    //ServerRPC_ShowRobbyWidget();
}

// Called every frame
void APortalCollision::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //if(WidgetInstance)
    if (WidgetInstance->ChangeReady1Test == true)
    {
        
        MyGameMode->bIsReadyToReady=true;

    }
    
}

bool APortalCollision::IsServer() const
{
    return GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer;
}

void APortalCollision::OnBeginOverlapPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->GetName().Contains("Player"))
    {
        ServerRPC_ShowRobbyWidget();
        
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
    if (IsServer())
    {
        // 모든 플레이어의 위치를 저장
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            APlayerController* PlayerController = It->Get();
            if (PlayerController && PlayerController->GetCharacter())
            {
                FVector PlayerLocation = PlayerController->GetCharacter()->GetActorLocation();
                PlayerStartLocations.Add(PlayerController, PlayerLocation);
            }
        }

        // 서버 트래블 호출
        FString MapName = TEXT("/Game/KMS_AI/BossMap/Dungeon2?listen");
        GetWorld()->ServerTravel(MapName, true);
    }
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
    // 모든 플레이어에 대해 위젯 제거
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PlayerController = It->Get();
        if (PlayerController && WidgetInstance)
        {
            WidgetInstance->RemoveFromParent();
            PlayerController->bShowMouseCursor = false;
            PlayerController->bEnableClickEvents = false;
            PlayerController->bEnableTouchEvents = false;
        }
    }
    
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
            APlayerController* PlayerController = It->Get();
            if (PlayerController)
            {
                // 위젯 인스턴스 생성
                WidgetInstance = CreateWidget<UPortalRobbyWidget>(PlayerController, portalRobbyWidget);
                if (WidgetInstance)
                {
                    // 위젯을 화면에 추가
                    WidgetInstance->AddToViewport();

                    PlayerController->bShowMouseCursor = true;
                    PlayerController->bEnableClickEvents = true;
                    PlayerController->bEnableTouchEvents = true;
                }
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
