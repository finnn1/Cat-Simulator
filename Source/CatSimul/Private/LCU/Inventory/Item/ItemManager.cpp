// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Inventory/Item/ItemManager.h"

#include "LCU/Inventory/UItem.h"
#include "LCU/Inventory/UItemDataComponent.h"
#include "LCU/Inventory/Item/UFoodItem.h"

// Sets default values
AItemManager::AItemManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItemManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemManager::CheckRespawnItem(AUItem* Item)
{
	if(AUFoodItem* foodItem = Cast<AUFoodItem>(Item))
	{
		if(!foodItem->FoodAttributes.HasFoodProperty(EFoodProperty::Respawn)) return;
 
		// 아이템의 위치와 클래스를 저장
		FItemRespawnData RespawnInfo;
		RespawnInfo.Location = Item->GetActorLocation();
		RespawnInfo.ItemClass = Item->GetClass();
		RespawnInfo.RespawnTime = 10.0f; // 30초 뒤 리스폰
 
		FName itemID = Item->ItemDataComponent->ItemID.RowName;
		// 맵에 데이터 추가
		RespawnData.Add(itemID, RespawnInfo);

		FTimerHandle RespawnTimerHandle;
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AItemManager::RespawnItem, itemID);
		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, TimerDelegate, 10.f, false);
	}
}

void AItemManager::RespawnItem(FName ItemID)
{
	// 리스폰 데이터 가져오기
	if (RespawnData.Contains(ItemID))
	{
		FItemRespawnData RespawnInfo = RespawnData[ItemID];

		// 아이템 스폰
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* SpawnedItem = GetWorld()->SpawnActor<AActor>(RespawnInfo.ItemClass, RespawnInfo.Location, FRotator::ZeroRotator, SpawnParams);

		if (SpawnedItem)
		{
			// 스폰 성공 시 데이터와 타이머 핸들 제거
			RespawnData.Remove(ItemID);
		}
	}
}

