// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Inventory/UI/UInventorySlot.h"

#include "EditorCategoryUtils.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "LCU/Inventory/FItemStruct.h"
#include "LCU/Inventory/InventorySystem.h"
#include "LCU/Inventory/UI/ActionMenu.h"
#include "LCU/Inventory/UI/UDDInventory.h"
#include "LCU/Inventory/UI/UDragPreview.h"


void UUInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UUInventorySlot::NativePreConstruct()
{
	Super::NativePreConstruct();

	const FString ContextString(TEXT("GetMaxStackSize Context"));

	FItemStruct* RowItemData = Item_DataTable->FindRow<FItemStruct>(ItemID, ContextString);

	if(RowItemData)
	{
		FText quantity = UKismetTextLibrary::Conv_IntToText(Quantity);
		IMG_Icon->SetBrushFromTexture(RowItemData->Icon);
		TXT_Quantity->SetText(quantity);		
		IMG_Icon->SetVisibility(ESlateVisibility::Visible);
		SizeBox_Quantity->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		IMG_Icon->SetVisibility(ESlateVisibility::Hidden);
		SizeBox_Quantity->SetVisibility(ESlateVisibility::Hidden);
	}
}

FReply UUInventorySlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//if(ItemID == "")
	//{
	//	return FReply::Unhandled();
	//}
	//
	//if(InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	//{
	//	if(ActionMenu) ActionMenu->RemoveFromParent();
	//	if(ActionMenuFactory)
	//	{
	//		ActionMenu = Cast<UActionMenu>(CreateWidget(GetWorld(), ActionMenuFactory));
	//		ActionMenu->Init(InventoryComp, ContentIndex);
	//		ActionMenu->AddToViewport();
	//	}
	//}
	//else if(InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	//{
	//	
	//}
	
	if(ItemID == "")
	{
		return FReply::Unhandled();
	}
	if(InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		InventoryComp->RemoveFromInventory(ContentIndex, false, true);
	}
	else if(InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		UseItem();
	}
	
	
	
	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}

void UUInventorySlot::Init(FName itemID, int32 quatity, UInventorySystem* inventorySystem, int32 contentIndex)
{
	ItemID = itemID;
	Quantity = quatity;
	InventoryComp = inventorySystem;
	ContentIndex = contentIndex;

	const FString ContextString(TEXT("GetMaxStackSize Context"));

	FItemStruct* RowItemData = Item_DataTable->FindRow<FItemStruct>(ItemID, ContextString);

	if(RowItemData)
	{
		FText quantity = UKismetTextLibrary::Conv_IntToText(Quantity);
		IMG_Icon->SetBrushFromTexture(RowItemData->Icon);
		TXT_Quantity->SetText(quantity);		
		IMG_Icon->SetVisibility(ESlateVisibility::Visible);
		SizeBox_Quantity->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		IMG_Icon->SetVisibility(ESlateVisibility::Hidden);
		SizeBox_Quantity->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUInventorySlot::UseItem_Implementation()
{
	
}


