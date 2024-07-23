#include "PortalRobbyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MyGameModeBase.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

void UPortalRobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (button_Player1)
	{
		button_Player1->OnClicked.AddDynamic(this, &UPortalRobbyWidget::OnMyclickButtonPlayer1);
	}

	if (button_Player2)
	{
		button_Player2->OnClicked.AddDynamic(this, &UPortalRobbyWidget::OnMyclickButtonPlayer2);
	}
	MyGameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(this));
	
	
}

void UPortalRobbyWidget::ServerRPC_ShowReady_Implementation()
{
	bIsReadyTextPlayer1 = !bIsReadyTextPlayer1;
	MulticastRPC_ShowReady();
	//OnRep_ReadyTextPlayer1();
}

void UPortalRobbyWidget::MulticastRPC_ShowReady_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("IS999"));
	if (readyButtonText1)
	{
		if (bIsReadyTextPlayer1)
		{
			readyButtonText1->SetText(FText::FromString(TEXT("Ready!")));
		}
		else
		{
			readyButtonText1->SetText(FText::GetEmpty());
		}
		ChangeReady1Test = true;
	}


	//MyGameMode->UICheckReady1 = true;
}

void UPortalRobbyWidget::ServerRPC_ShowReady2_Implementation()
{
	bIsReadyTextPlayer2 = !bIsReadyTextPlayer2;
	//MulticastRPC_ShowReady2();
	OnRep_ReadyTextPlayer2();
}

void UPortalRobbyWidget::MulticastRPC_ShowReady2_Implementation()
{
	
}


/// <summary>
///클릭
/// </summary>
void UPortalRobbyWidget::OnMyclickButtonPlayer1()
{
	//if (GetOwningPlayer())
	{
		
		ServerRPC_ShowReady();
		
	}
	//MyGameMode->
}

void UPortalRobbyWidget::OnMyclickButtonPlayer2()
{
	//if (GetOwningPlayer())
	{
		ServerRPC_ShowReady2();
	}
}

void UPortalRobbyWidget::OnRep_ReadyTextPlayer1()
{
	
	
}

void UPortalRobbyWidget::SetText()
{
	readyButtonText1->SetText(FText::FromString(TEXT("Ready!")));
}

void UPortalRobbyWidget::NoSetText()
{
	readyButtonText1->SetText(FText::GetEmpty());
}

void UPortalRobbyWidget::OnRep_ReadyTextPlayer2()
{
	if (readyButtonText2)
	{
		if (bIsReadyTextPlayer2)
		{
			readyButtonText2->SetText(FText::FromString(TEXT("Ready!")));

		}
		else
		{
			readyButtonText2->SetText(FText::GetEmpty());
		}
	}


}

void UPortalRobbyWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPortalRobbyWidget, bIsReadyTextPlayer1);
	DOREPLIFETIME(UPortalRobbyWidget, bIsReadyTextPlayer2);
}
