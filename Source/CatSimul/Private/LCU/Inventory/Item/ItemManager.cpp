// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Inventory/Item/ItemManager.h"

#include "LCU/Inventory/FItemStruct.h"
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
		const FString ContextString(TEXT("GetMaxStackSize Context"));
		FName itemID = Item->ItemDataComponent->ItemID.RowName;
		FItemStruct* RowItemData = ItemDataTable->FindRow<FItemStruct>(itemID, ContextString);

		// 리스폰이 불가한 아이템이거나 해당 아이템의 정보가 없거나 이미 픽업 한 아이템을 다시 만진다면 그 아이템은 리스폰하지 않습니다.
		if(!(RowItemData && RowItemData->ItemProperty.Contains(EItemProperty::RESPAWNABLE) && Item->GetCanRespawn())) return;
 
		// 아이템의 위치와 클래스를 저장
		FItemRespawnData RespawnInfo;
		RespawnInfo.Location = Item->GetActorLocation();
		RespawnInfo.ItemClass = Item->GetClass();
		RespawnInfo.RespawnTime = RowItemData->ItemProperty[EItemProperty::RESPAWNABLE]; 
 
		// 맵에 데이터 추가
		RespawnData.Add(itemID, RespawnInfo);

		FTimerHandle RespawnTimerHandle;
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AItemManager::RespawnItem, Item->ItemDataComponent->ItemID.RowName);
		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, TimerDelegate, RespawnInfo.RespawnTime, false);
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

