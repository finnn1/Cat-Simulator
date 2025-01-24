// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LCU/Inventory/UItem.h"
#include "UsedItem.generated.h"

UCLASS()
class CATSIMUL_API AUsedItem : public AUItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUsedItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
