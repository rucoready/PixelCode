// Fill out your copyright notice in the Description page of Project Settings.


#include "PCodePlayerController.h"

void APCodePlayerController::OpenUI(bool bOpen)
{
	if(bOpen)
	{
		SetInputMode(FInputModeGameAndUI());
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
	}

	SetShowMouseCursor(bOpen);
}
