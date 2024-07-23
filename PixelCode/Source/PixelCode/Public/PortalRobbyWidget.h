#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PortalRobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class PIXELCODE_API UPortalRobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	// Button and text block widgets
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* button_Player1;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* button_Player2;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* readyButtonText1;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* readyButtonText2;

	// Server-side state
	UPROPERTY(ReplicatedUsing = OnRep_ReadyTextPlayer1)
	bool bIsReadyTextPlayer1 = false;

	UPROPERTY(ReplicatedUsing = OnRep_ReadyTextPlayer2)
	bool bIsReadyTextPlayer2 = false;

	class AMyGameModeBase* MyGameMode;

	// RPC Functions
	UFUNCTION(Server, Reliable)
	void ServerRPC_ShowReady();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ShowReady();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ShowReady2();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ShowReady2();

	// Button click handlers
	UFUNCTION()
	void OnMyclickButtonPlayer1();

	UFUNCTION()
	void OnMyclickButtonPlayer2();


	// Replication functions
	UFUNCTION()
	void OnRep_ReadyTextPlayer1();

	UFUNCTION()
	void SetText();

	UFUNCTION()
	void NoSetText();

	UFUNCTION()
	void OnRep_ReadyTextPlayer2();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool ChangeReady1Test = false;
};
