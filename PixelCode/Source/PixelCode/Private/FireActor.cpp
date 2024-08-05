#include "FireActor.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Player/PixelCodeCharacter.h"
#include "PCodePlayerController.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AFireActor::AFireActor()
{
    PrimaryActorTick.bCanEverTick = true;

    // Initialize components
    sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComponent"));
    SetRootComponent(sceneComponent);

    sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
    sphereComp->SetupAttachment(RootComponent);
    sphereComp->InitSphereRadius(200.0f); 
    sphereComp->SetCollisionProfileName(TEXT("OverlapAllDynamic")); 

    fireNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("fireNiagara"));
    fireNiagara->SetupAttachment(RootComponent);

    bIsOverlapping = false;
    OverlappingActor = nullptr;
}

// Called when the game starts or when spawned
void AFireActor::BeginPlay()
{
    Super::BeginPlay();
    sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFireActor::OnBeginOverlapCollision);
    sphereComp->OnComponentEndOverlap.AddDynamic(this, &AFireActor::OnEndOverlapCollision);
}

// Called every frame
void AFireActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AFireActor::OnBeginOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && OtherActor->IsA(APixelCodeCharacter::StaticClass()))
    {
        OverlappingActor = OtherActor;
        bIsOverlapping = true;

        GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AFireActor::ApplyDamage, 0.5f, true);
    }
}

void AFireActor::OnEndOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor == OverlappingActor)
    {
        bIsOverlapping = false;
        OverlappingActor = nullptr;

        GetWorldTimerManager().ClearTimer(DamageTimerHandle);
    }
}

void AFireActor::ApplyDamage()
{
    if (OverlappingActor && bIsOverlapping)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireDamageSound, GetActorLocation());
        ApplyDamageToTarget(OverlappingActor, 5.0f);

        // OverlappingActor의 컨트롤러를 가져옴
        if (Controller)
        {
            Controller = OverlappingActor->GetInstigatorController();
            if (Controller && Controller->IsLocalController())
            {
                APCodePlayerController* pc = Cast<APCodePlayerController>(Controller);
                if (pc != nullptr)
                {
              
                    pc->ClientStartCameraShake(cameraShakeFire);
                }
            }

            GetWorldTimerManager().SetTimer(FireDotDamageHandle, this, &AFireActor::DisappearFire, 3.0f, false);
        }
        
    }
}

void AFireActor::ApplyDamageToTarget(AActor* OtherActor, float DamageAmount)
{
    if (IsValid(OtherActor))
    {
        Pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
        UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, Pc, Player, UDamageType::StaticClass());
    }
}

void AFireActor::DisappearFire()
{
    if (particleComponent)
    {
        particleComponent->DestroyComponent(true);
    }
}
