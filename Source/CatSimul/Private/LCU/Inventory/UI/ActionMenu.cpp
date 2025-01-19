// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Inventory/UI/ActionMenu.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"

void UActionMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	FVector2D pos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	auto canvasSlot =  UWidgetLayoutLibrary::SlotAsCanvasSlot(BOX_ActionMenu);
	canvasSlot->SetPosition(pos);
}

void UActionMenu::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	RemoveFromParent();
}

void UActionMenu::Init(UInventorySystem* inventorySystem, int32 index)
{
	InventorySystem = inventorySystem;
	Index = index;
}
