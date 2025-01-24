// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Inventory/Item/UsedItem.h"


// Sets default values
AUsedItem::AUsedItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUsedItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUsedItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
