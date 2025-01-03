// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Inventory/UI/UInventorySlot.h"

#include "Components/SizeBox.h"
#include "LCU/Inventory/FItemStruct.h"


void UUInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	const FString ContextString(TEXT("GetMaxStackSize Context"));

	FItemStruct* RowItemData = Item_DataTable->FindRow<FItemStruct>(ItemID, ContextString);

	if(RowItemData)
	{
		SizeBox_Quantity->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		
	}
}
