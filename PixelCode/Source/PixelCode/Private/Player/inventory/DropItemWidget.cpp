// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/inventory/DropItemWidget.h"
#include "Player/inventory/itemDragDropOperation.h"

bool UDropItemWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UitemDragDropOperation* DragWidgetResult = Cast<UitemDragDropOperation>(InOperation);

	if (!IsValid(DragWidgetResult))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Cast returned null."))
			return false;
	}

	/*const FVector2D DragWindowOffSet = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	const FVector2D DragWindowOffSetResult = DragWindowOffSet - DragWidgetResult->DragOffset;

	DragWidgetResult->WidgetReference->AddToViewport();
	DragWidgetResult->WidgetReference->SetVisibility(ESlateVisibility::Visible);
	DragWidgetResult->WidgetReference->SetPositionInViewport(DragWindowOffSetResult, false);*/


	return true;
}