#include "PortalRobbyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MyGameModeBase.h"
#include "Net/UnrealNetwork.h"
#include "PortalCollision.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PixelCodeCharacter.h"
#include "PCodePlayerController.h"
#include "LoadingWidget1.h"
#include "Animation/WidgetAnimation.h"

void UPortalRobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (button_Player1)
	{
		button_Player1->OnClicked.AddDynamic(this, &UPortalRobbyWidget::OnMyclickButtonPlayer1);
	}
	if (button_Exit)
	{
		
		button_Exit->OnClicked.AddDynamic(this, &UPortalRobbyWidget::ExitWidget);
	}

// 	if (button_Player2)
// 	{
// 		button_Player2->OnClicked.AddDynamic(this, &UPortalRobbyWidget::OnMyclickButtonPlayer2);
// 	}
	MyGameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(this));
	PortalActor = Cast<APortalCollision>(UGameplayStatics::GetActorOfClass(this, APortalCollision::StaticClass()));
	
	
}

void UPortalRobbyWidget::ServerTravel()
{
	GetWorld()->ServerTravel(TEXT("/Game/KMS_AI/BossMap/Dungeon2?listen"));
	
	
	//Pc->UpdateGameInstanceInventory();

	
	//GetWorld()->ServerTravel(TEXT("/Game/KMS_AI/BossMap/BossMap2?Listen"));
	if (PCodePlayerController)
	{
		PCodePlayerController->ServerRPC_HideWidgetLoading1();

	}

}

void UPortalRobbyWidget::StartCountdown()
{
	countdownTime = 5; // 5초부터 시작
	if (startCount)
	{
		startCount->SetText(FText::AsNumber(countdownTime));
	}

	// 매초마다 UpdateCountdown을 호출하는 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(countdownTimerHandle, this, &UPortalRobbyWidget::UpdateCountdown, 1.0f, true);
}

void UPortalRobbyWidget::UpdateCountdown()
{
	countdownTime--;

	if (startCount)
	{
		
		startCount->SetText(FText::AsNumber(countdownTime));
		
	}

	PlayCountdownAnimation();

	if (countdownTime <= 0)
	{
		
		// 카운트다운이 끝났을 때 호출할 코드
		GetWorld()->GetTimerManager().ClearTimer(countdownTimerHandle);
		
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			// PCodePlayerController 타입으로 캐스팅
			PCodePlayerController = Cast<APCodePlayerController>(It->Get());
			if (PCodePlayerController)
			{
				PCodePlayerController->ServerRPC_CreateWidgetLoading1();
				
			}
		}

		GetWorld()->GetTimerManager().SetTimer(timerhandle_ServerTravel, this, &UPortalRobbyWidget::ServerTravel, 7.0f, false);






		//GetWorld()->ServerTravel(TEXT("/Game/KMS_AI/BossMap/Dungeon2?listen"));
	}
}

void UPortalRobbyWidget::PlayCountdownAnimation()
{
	
	PlayAnimation(CountAnimations);
	
}

void UPortalRobbyWidget::ExitWidget()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		// PCodePlayerController 타입으로 캐스팅
		PCodePlayerController = Cast<APCodePlayerController>(It->Get());
		if (PCodePlayerController)
		{
			PCodePlayerController->ServerRPC_HideWidgetRobbyWidget();
		}
	}
}

void UPortalRobbyWidget::ServerRPC_ShowReady_Implementation()
{
	bIsReadyTextPlayer1 = !bIsReadyTextPlayer1;
	MulticastRPC_ShowReady();
	MyGameMode->bIsReadyToReady=true;
	//OnRep_ReadyTextPlayer1();
}

void UPortalRobbyWidget::MulticastRPC_ShowReady_Implementation()
{
	
	ChangeReady1Test = true;
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
	
	
	MyGameMode->bIsReadyToReady=true;
	ServerRPC_ShowReady();
 	StartCountdown();
 	PlayCountdownAnimation();

	
	
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
	
	//readyButtonText1->SetText(FText::FromString(TEXT("Ready!")));
	
}

void UPortalRobbyWidget::NoSetText()
{
	readyButtonText1->SetText(FText::GetEmpty());
}

void UPortalRobbyWidget::OnRep_ReadyTextPlayer2()
{
// 	if (readyButtonText2)
// 	{
// 		if (bIsReadyTextPlayer2)
// 		{
// 			readyButtonText2->SetText(FText::FromString(TEXT("Ready!")));
// 
// 		}
// 		else
// 		{
// 			readyButtonText2->SetText(FText::GetEmpty());
// 		}
// 	}


}

void UPortalRobbyWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPortalRobbyWidget, bIsReadyTextPlayer1);
	DOREPLIFETIME(UPortalRobbyWidget, bIsReadyTextPlayer2);
}

void UPortalRobbyWidget::NormalChangeButton1()
{
	
// 	if (bIsReadyTextPlayer1)
// 	{
// 		readyButtonText1->SetText(FText::FromString(TEXT("Ready!")));
// 	}
// 	else
// 	{
// 		readyButtonText1->SetText(FText::GetEmpty());
// 	}

	
}
