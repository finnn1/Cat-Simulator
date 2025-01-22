// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Inventory/UI/UInventoryGrid.h"

#include "Components/WrapBox.h"
#include "LCU/Inventory/InventorySystem.h"
#include "LCU/Inventory/UI/UInventorySlot.h"

void UUInventoryGrid::GridSetting()
{
	if(InventorySystem)
	{
		for(int32 i = 0; i < InventorySystem->GetContent().Num(); i++)
		{
			UUserWidget* createSlot = CreateWidget(GetWorld(), SlotFactory);
			UUInventorySlot* slot = Cast<UUInventorySlot>(createSlot);

			if(slot)
			{
				FName itemID = InventorySystem->GetContent()[i].ItemID;
				int32 quantity = InventorySystem->GetContent()[i].Quantity;
				slot->Init(itemID, quantity, InventorySystem, i, this);
				Box_Grid->AddChildToWrapBox(slot);
			}
		}
	}
}

void UUInventoryGrid::NativePreConstruct()
{
	Super::NativePreConstruct();

	GridSetting();
}

void UUInventoryGrid::DisplayInventory(UInventorySystem* inventorySystem)
{
	InventorySystem = inventorySystem;
	Box_Grid->ClearChildren();
	
	GridSetting();
}
