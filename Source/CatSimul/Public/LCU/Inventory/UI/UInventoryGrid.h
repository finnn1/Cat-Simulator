// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UInventoryGrid.generated.h"

class UInventorySystem;

UCLASS()
class CATSIMUL_API UUInventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	void GridSetting();
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable)
	void DisplayInventory(UInventorySystem* inventorySystem);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UInventorySystem* InventorySystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<class UUInventorySlot> SlotFactory;

public:
	// 위젯
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UWrapBox* Box_Grid;
};
