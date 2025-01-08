// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Inventory/UI/UDragPreview.h"

#include "Components/Image.h"
#include "LCU/Inventory/FItemStruct.h"

void UUDragPreview::NativePreConstruct()
{
	Super::NativePreConstruct();
		
	if(ItemDataTable && ItemID.IsValid())
	{
		const FString ContextString(TEXT("GetMaxStackSize Context"));
		
		FItemStruct* itemStruct = ItemDataTable->FindRow<FItemStruct>(ItemID, ContextString);

		if(itemStruct)
		{
			IMG_DragIcon->SetBrushFromTexture(itemStruct->Icon);
		}
	}
}

void UUDragPreview::Init()
{
	if(ItemDataTable && ItemID.IsValid())
	{
		const FString ContextString(TEXT("GetMaxStackSize Context"));
		
		FItemStruct* itemStruct = ItemDataTable->FindRow<FItemStruct>(ItemID, ContextString);

		if(itemStruct)
		{
			IMG_DragIcon->SetBrushFromTexture(itemStruct->Icon);
		}
	}
}
