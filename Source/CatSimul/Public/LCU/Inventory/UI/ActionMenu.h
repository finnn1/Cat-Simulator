// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionMenu.generated.h"

class UVerticalBox;
class UInventorySystem;

/**
 * 
 */
UCLASS()
class CATSIMUL_API UActionMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	void Init(UInventorySystem* inventorySystem, int32 index);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionMenu", meta=(BindWidget))
	UVerticalBox* BOX_ActionMenu;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Menu")
	UInventorySystem* InventorySystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Menu")
	int32 Index;
};
