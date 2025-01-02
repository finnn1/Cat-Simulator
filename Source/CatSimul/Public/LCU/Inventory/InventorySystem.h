// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSlotStruct.h"
#include "Components/ActorComponent.h"
#include "InventorySystem.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class CATSIMUL_API UInventorySystem : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventorySystem();

	void RemoveFromInventory();
	void AddToStack(int32 index, int32 quantity);
	bool CreateToStack(FName itemID, int32 quantity);
	int32 AddToInventory(FName itemID, int32 quantity, bool& bsuccess);
	int32 FindSlot(FName itemID, bool& bSuccess);
	int32 GetMaxStackSize(FName itemID);
	int32 AnyEmptySlotIndex(bool& bSuccess);
	

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta=(AllowPrivateAccess=true))
	int32 InventorySize;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta=(AllowPrivateAccess=true))
	TArray<FSlotStruct> Content;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta=(AllowPrivateAccess=true))
	class UDataTable* ItemDataTable;

	
};
