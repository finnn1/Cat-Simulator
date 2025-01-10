// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "UDDInventory.generated.h"

/**
 * 
 */
UCLASS()
class CATSIMUL_API UUDDInventory : public UDragDropOperation
{
	GENERATED_BODY()

public:



public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	class UInventorySystem* InventorySystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 ContentIndex;
};
