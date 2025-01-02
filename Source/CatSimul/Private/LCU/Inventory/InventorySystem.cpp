// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Inventory/InventorySystem.h"

#include "LCU/Inventory/FItemStruct.h"


// Sets default values for this component's properties
UInventorySystem::UInventorySystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	

	// ...
}

int32 UInventorySystem::AddToInventory(FName itemID, int32 quantity, bool& bsuccess)
{
	if(quantity<=0)
	{
		bsuccess = false;
		return -1;
	}
	bsuccess = true;
	int32 localQuantity = quantity;
	bool foundSlot = false;
	
	while(localQuantity>0)
	{
		int32 stackSize = FindSlot(itemID, foundSlot);

		// 현재 해당 아이템을 사용하는 슬롯이 있습니다.
		if(foundSlot)
		{
			// 슬롯에 스택을 추가하고 넣을 수량을 -1
			AddToStack(stackSize, 1);
			localQuantity--;
		}
		// 현재 해당 아이템을 사용하는 슬롯이 없거나 갯수가 꽉찼습니다.
		else
		{
			// 더 이상 비어있는 슬롯이 없어요
			if(!AnyEmptySlotIndex(bsuccess))
			{
				bsuccess = false;
				break;
			}

			// 해당 아이템을 사용할 추가 슬롯을 설정하고 스택을 1로 해줍니다.
			if(CreateToStack(itemID,1))
			{
				localQuantity--;
			}
			// 해당 아이템을 만들 슬롯조차 더 이상 없다면 나갑니다.
			else
			{
				bsuccess = false;
				break;
			}
			
		}
	}

	return localQuantity;
}

void UInventorySystem::RemoveFromInventory()
{
	
}

void UInventorySystem::AddToStack(int32 index, int32 quantity)
{
	Content[index].Quantity += quantity;	
}

bool UInventorySystem::CreateToStack(FName itemID, int32 quantity)
{
	bool hasAnyEmptySlot = false;
	int32 newSlotIndex = AnyEmptySlotIndex(hasAnyEmptySlot);
	if(hasAnyEmptySlot)
	{
		Content[newSlotIndex].ItemID = itemID;
		Content[newSlotIndex].Quantity = quantity;
		return true;
	}
	return false;
}

int32 UInventorySystem::FindSlot(FName itemID, bool& bSuccess)
{
	for(int32 i = 0; i < Content.Num(); ++i)
	{
		if(Content[i].ItemID == itemID)
		{
			int32 maxStackSize = GetMaxStackSize(itemID);
			if(maxStackSize > Content[i].Quantity)
			{
				bSuccess = true;
				return i;
			}
		}
	}
	return -1;      
}

int32 UInventorySystem::GetMaxStackSize(FName itemID)
{
	if(!ItemDataTable) return -1;
	
	const FString ContextString(TEXT("GetMaxStackSize Context"));

	FItemStruct* RowItemData = ItemDataTable->FindRow<FItemStruct>(itemID, ContextString);

	if(RowItemData)
	{
		return RowItemData->StackSize;
	}
	return -1;
}

int32 UInventorySystem::AnyEmptySlotIndex(bool& bSuccecc)
{
	for(int32 i = 0; i < Content.Num(); ++i)
	{
		if(Content[i].Quantity == 0)
		{
			bSuccecc = true;
			return i;
		}
	}
	return -1;
}

