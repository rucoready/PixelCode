#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/PixelCodeCharacter.h"
#include "PortalCollision.generated.h"

UCLASS()
class PIXELCODE_API APortalCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortalCollision();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="MySettings")
	class UBoxComponent* boxCollision;

	class AMyGameModeBase* MyGameMode;

	UFUNCTION(BlueprintCallable, Category="Networking")
    bool IsServer() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UPortalRobbyWidget> portalRobbyWidget;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* portalCreateSound;

	

	bool makeOnceUI = false;

	bool bReady = false;

	class UPortalRobbyWidget* WidgetInstance;

	class APixelCodeCharacter* Pc;

	TMap<APlayerController*, FVector> PlayerStartLocations;

	UFUNCTION()
	void OnBeginOverlapPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OneEndOverlapPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ServerTravel();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ShowRobbyWidget();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ShowRobbyWidget();

	UFUNCTION(Server, Reliable)
	void ServerRPC_HideRobbyWidget();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_HideRobbyWidget();

	UPROPERTY(ReplicatedUsing = OnRep_Ready)
	bool bIsReady = false;

	UFUNCTION()
	void OnRep_Ready();

	UFUNCTION()
	void ChangeBisReady(bool newReady);
	

};
