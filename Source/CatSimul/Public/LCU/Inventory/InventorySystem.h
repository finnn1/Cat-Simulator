// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSlotStruct.h"
#include "Components/ActorComponent.h"
#include "InventorySystem.generated.h"

struct FItemStruct;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class CATSIMUL_API UInventorySystem : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventorySystem();

	// Remove Item
	UFUNCTION(BlueprintCallable)
	void RemoveFromInventory(int32 index, bool removeWholeStack, bool isConsumed);

	UFUNCTION(BlueprintCallable)
	void Remove(int32 index, bool removeWholeStack, bool isConsumed);
	
	// Drop Item
	void DropItem(FName itemID, int32 quantity);

	// Increase Item Stack
	void AddToStack(int32 index, int32 quantity);

	//  ItemSlot into Inventory
	bool CreateToStack(FName itemID, int32 quantity);

	// 
	UFUNCTION(BlueprintCallable)
	int32 AddToInventory(FName itemID, int32 quantity, bool& bsuccess);
	int32 FindSlot(FName itemID, bool& bSuccess);
	int32 GetMaxStackSize(FName itemID);
	int32 AnyEmptySlotIndex(bool& bSuccess);
	
	void UseItem(int32 index);

	// Get, Set
	TArray<FSlotStruct> GetContent() {return Content;}

	FItemStruct* GetItemData(FName itemID);

	FVector GetDropLocation();

	
	
	// Get, Set End

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	AActor* ItemInLeftHand;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	AActor* ItemInRightHand;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta=(AllowPrivateAccess=true))
	int32 InventorySize;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta=(AllowPrivateAccess=true))
	TArray<FSlotStruct> Content;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta=(AllowPrivateAccess=true))
	class UDataTable* ItemDataTable;

	
};
