// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Slider.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/EditableText.h>
#include "PCodeGameInstance.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetSwitcher.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ScrollBox.h>
#include "RoomInfoUI.h"


void ULobbyWidget::NativeConstruct()
{
	// 게임 인스턴스 채우기
	gi = Cast<UPCodeGameInstance>(GetWorld()->GetGameInstance());


	gi->OnMySessionSearchCompleteDelegate.AddDynamic(this, &ULobbyWidget::AddRoomInfoUI);
	

	// 버튼
	Button_CreatRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickCreateRoom);
	Button_GoCreateRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoCreateRoom);
	Button_GoFindRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickGoFindRoom);
	Button_FindRoom->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickFindRoom);
	

	Button_Back1->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickBackRoom);
	Button_Back2->OnClicked.AddDynamic(this, &ULobbyWidget::OnMyClickBackRoom);
	
	SwitcherUI->SetActiveWidgetIndex(0);

	Slider_PlayerCount->SetValue(FCString::Atof(*Text_PlayerCount->GetText().ToString()));

	Slider_PlayerCount->OnValueChanged.AddDynamic(this, &ULobbyWidget::OnValueChanged);

	SwitcherUI->SetActiveWidgetIndex(0);
}

void ULobbyWidget::OnMyClickBackRoom()
{
	SwitcherUI->SetActiveWidgetIndex(0);
}

void ULobbyWidget::OnMyClickGoCreateRoom()
{
	SwitcherUI->SetActiveWidgetIndex(1);
}

void ULobbyWidget::OnMyClickGoFindRoom()
{
	/*if (false == SetSessionName())
		return;*/

	SwitcherUI->SetActiveWidgetIndex(2);
	// 방찾기 메뉴 진입시 찾기를 시도하고싶다.
	if (gi)
	{
		gi->FindOtherSessions();
	}
}



void ULobbyWidget::AddRoomInfoUI(const struct FSessionInfo& info)
{
	// RoomInfoUIFactory를 이용해서 위젯을 만들고
	auto ui = CreateWidget<URoomInfoUI>(this, roominfoUIFactory);
	// info를 Setup함수를 통해 전달하고싶다.
	ui->Setup(info);
	// 생성한 위젯을 ScrollBox_RoomList에 붙이고싶다.
	ScrollBox_RoomList->AddChild(ui);
}

//bool ULobbyWidget::SetSessionName()
//{
//	// 해야할일!
//		// badword 체크!
//	FString str = Edit_SessionName->GetText().ToString();
//	//str = str.TrimStartAndEnd();
//	//str = str.Replace(TEXT("|"), TEXT(""));
//	//str.Split()
//	//str.Replace()
//	// 입력한 것이 없다면
//	if (str.IsEmpty()) {
//		return false;
//	}
//}

void ULobbyWidget::OnMyClickCreateRoom()
{
	if(gi)
	{
		int32 count = Slider_PlayerCount->GetValue();
		gi->CreateMySession(Edit_RoomName->GetText().ToString(),count);
	}
}

void ULobbyWidget::OnMyClickFindRoom()
{
	// 기존의 목록을 삭제하고싶다.
	ScrollBox_RoomList->ClearChildren();
	// gi의 FindOtherSessions를 호출하고싶다.
	gi->FindOtherSessions();
}

void ULobbyWidget::OnValueChanged(float value)
{
	Text_PlayerCount->SetText(FText::AsNumber(value));
}