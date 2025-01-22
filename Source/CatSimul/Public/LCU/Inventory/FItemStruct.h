// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FItemStruct.generated.h"

class AUItem;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None UMETA(DisplayName = "None"),
	Consume UMETA(DisplayName = "Consume"),
	Equip UMETA(DisplayName = "Equip"),
};

UENUM(BlueprintType)
enum class EItemProperty : uint8
{
	NONE UMETA(DisplayName = "None"),
	HUNGER UMETA(DisplayName = "Hunger"),
	RESPAWNABLE UMETA(DisplayName = "Respawn"),
	CUT UMETA(DisplayName = "Cut"),
};

USTRUCT(BlueprintType, Blueprintable)
struct FItemStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FName Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TSubclassOf<AUItem> ItemObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 StackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EItemType ItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TMap<EItemProperty, float> ItemProperty;
	
	FItemStruct()
		: Name(NAME_None), Description(""), Icon(nullptr), StackSize(0), ItemType(EItemType::None), ItemProperty()
	{
		
	}
};