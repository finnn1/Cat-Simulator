// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UDragPreview.generated.h"

/**
 * 
 */
UCLASS()
class CATSIMUL_API UUDragPreview : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;

	void Init();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* IMG_DragIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDataTable* ItemDataTable;
};
