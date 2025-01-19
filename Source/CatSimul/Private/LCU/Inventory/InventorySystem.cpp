// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Inventory/InventorySystem.h"

#include "Kismet/KismetMathLibrary.h"
#include "LCU/Inventory/FItemStruct.h"
#include "LCU/Inventory/UItem.h"
#include "LCU/Inventory/Item/UFoodItem.h"
#include "Windows/WindowsApplication.h"


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
			if(AnyEmptySlotIndex(bsuccess))
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

void UInventorySystem::RemoveFromInventory(int32 index, bool removeWholeStack, bool isConsumed)
{
	FName localID = Content[index].ItemID;
	int32 localQuantity = Content[index].Quantity;

	if(localQuantity == 1 || removeWholeStack)
	{
		Content[index].ItemID = FName();
		Content[index].Quantity = 0;
		if(isConsumed)
		{
			DropItem(localID, localQuantity);
		}
	}
	else
	{
		Content[index].Quantity--;
		if(isConsumed)
		{
			DropItem(localID, 1);
		}
	}
}

void UInventorySystem::Remove(int32 index, bool removeWholeStack, bool isConsumed)
{
	
}

void UInventorySystem::DropItem(FName itemID, int32 quantity)
{
	FItemStruct* localData = GetItemData(itemID);
	if(localData)
	{
		for(int i = 0; i < quantity -1; ++i)
		{
			// 드롭 위치 가져오기
			FVector DropLocation = GetDropLocation();

			// 스폰 트랜스폼 설정 
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(DropLocation);

			// 아이템 스폰
			AActor* SpawnedActor = GetWorld()->SpawnActor<AUItem>(localData->ItemObject, SpawnTransform);
			if(AUFoodItem* AFoodItem = Cast<AUFoodItem>(SpawnedActor))
			{
				if(AFoodItem->FoodAttributes.FoodProperties.Contains(EFoodProperty::Respawn))
				{
					AFoodItem->FoodAttributes.FoodProperties.Remove(EFoodProperty::Respawn);
				}
			}
		}
	}
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

void UInventorySystem::UseItem(int32 index)
{
	FName localID = Content[index].ItemID;
	int32 localQuantity = Content[index].Quantity;

	
	
}

FItemStruct* UInventorySystem::GetItemData(FName itemID)
{
	const FString ContextString(TEXT("GetMaxStackSize Context"));

	FItemStruct* RowItemData = ItemDataTable->FindRow<FItemStruct>(itemID, ContextString);

	return RowItemData;
}

FVector UInventorySystem::GetDropLocation()
{
	if (AActor* Owner = GetOwner())
	{
		// 오너 위치 및 Forward 벡터 가져오기
		FVector ActorLocation = Owner->GetActorLocation();
		FVector ForwardVector = Owner->GetActorForwardVector();

		// 임의의 콘 벡터 계산 (90도 범위 내)
		float HalfAngleInDegrees = 90.0f;
		FVector RandomConeVector = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(ForwardVector, HalfAngleInDegrees);

		// 초기 드롭 위치 계산 (콘 방향으로 150 유닛 이동)
		FVector StartLocation = ActorLocation + (RandomConeVector * 150.0f);

		// 라인 트레이스의 끝 위치 설정 (아래 방향으로 5000 유닛)
		FVector EndLocation = StartLocation - FVector(0.0f, 0.0f, 5000.0f);

		// 라인 트레이스 결과
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Owner); // 소유자와 충돌 무시

		// 월드에서 라인 트레이스 실행
		UWorld* World = GetWorld();
		if (World && World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
		{
			// 충돌한 경우 충돌 지점을 드롭 위치로 반환
			return HitResult.Location;
		}

		// 충돌하지 않은 경우 기본 드롭 위치 반환
		return StartLocation;
	}

	// 소유자가 없는 경우 기본값 반환
	return FVector::ZeroVector;
}

