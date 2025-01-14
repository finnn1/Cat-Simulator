// Fill out your copyright notice in the Description page of Project Settings.


#include "LCU/Inventory/UItem.h"

#include "LCU/Inventory/UItemDataComponent.h"


// Sets default values
AUItem::AUItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemDataComponent = CreateDefaultSubobject<UUItemDataComponent>(TEXT("ItemDataComp"));
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AUItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

