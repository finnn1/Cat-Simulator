// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Inventory/UI/UInventorySlot.h"

#include <string>

#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "LCU/Inventory/FItemStruct.h"


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

void UUInventorySlot::Init(FName itemID, int32 quatity, UInventorySystem* inventorySystem)
{
	ItemID = itemID;
	Quantity = quatity;
	InventoryComp = inventorySystem;

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

