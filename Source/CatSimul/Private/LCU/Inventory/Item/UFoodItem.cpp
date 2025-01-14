// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Inventory/Item/UFoodItem.h"


bool FFoodAttribute::HasFoodOption(const EFoodOption& option)
{
	return FoodOptions.Contains(option);
}

// Sets default values
AUFoodItem::AUFoodItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AUFoodItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUFoodItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

