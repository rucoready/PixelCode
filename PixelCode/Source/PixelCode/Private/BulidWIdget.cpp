// Fill out your copyright notice in the Description page of Project Settings.


#include "BulidWIdget.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PixelCodeCharacter.h"
#include "BuildItemSlot.h"


void UBulidWIdget::NativeConstruct()
{

	Char = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
}
