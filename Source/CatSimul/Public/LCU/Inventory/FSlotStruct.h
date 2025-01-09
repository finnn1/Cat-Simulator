// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSlotStruct.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FSlotStruct
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	FSlotStruct()
		: ItemID(FName("")), Quantity(0)
	{
		
	}
};

