#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireActor.generated.h"

UCLASS()
class PIXELCODE_API AFireActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AFireActor();

    // Components
    UPROPERTY(EditAnywhere, Category = "MySettings")
    class USphereComponent* sphereComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USceneComponent* sceneComponent;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    class UNiagaraComponent* fireNiagara;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    class USoundBase* fireDamageSound;

    // Functions
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    class AController* Controller;

    UFUNCTION()
    void OnBeginOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnEndOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ApplyDamageToTarget(AActor* OtherActor, float DamageAmount);

    void DisappearFire();


    void ApplyDamage();

    // Timer handles
    FTimerHandle DamageTimerHandle;
    FTimerHandle FireDotDamageHandle;

    // State tracking
    bool bIsOverlapping;
    AActor* OverlappingActor;

    // Settings
    UPROPERTY(EditAnywhere, Category = "MySettings")
    class APlayerController* Pc;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    class APixelCodeCharacter* Player;

    UPROPERTY(EditAnywhere, Category = "Particles")
    class UParticleSystem* attachFireParticle;

    UParticleSystemComponent* particleComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
    TSubclassOf<UCameraShakeBase> cameraShakeFire;

    
};
