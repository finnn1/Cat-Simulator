// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UInventorySlot.generated.h"

class UBorder;
class UTextBlock;
class UImage;
class USizeBox;
class UDataTable;
class UInventorySystem;

UCLASS()
class CATSIMUL_API UUInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	UBorder* BRD_Border;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	UTextBlock* TXT_Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	UImage* IMG_Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	USizeBox* SizeBox_Quantity;

	// 아이템 관련 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInventorySystem* InventoryComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* Item_DataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;
};
