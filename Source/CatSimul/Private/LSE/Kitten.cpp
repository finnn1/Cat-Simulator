// Fill out your copyright notice in the Description page of Project Settings.


#include "LSE/Kitten.h"

#include "HairStrandsInterface.h"

// Sets default values
AKitten::AKitten()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("Kitten"));
	KittenMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Kitten"));
	RootComponent = KittenMesh;
	InterpSpeed = 10.0f;
	HasFoodCalStart = false;
	if(goOutKitten)
	{
		goOutKitten = GetWorld()->SpawnActor<AGoOutKitten>(AGoOutKitten::StaticClass(),	FVector(1528.948140,6598.352557,26.496031),FRotator(0.0f,0.0f,180.0f));
	}

}

// Called when the game starts or when spawned
void AKitten::BeginPlay()
{
	Super::BeginPlay();


	GetWorld()->GetTimerManager().SetTimer(
		FoodDecreaseTimerHandle,
		this,
		&AKitten::DecreaseKittenFood,
		0.2f,
		true
	);

	if(goOutKitten)
	{
		goOutKitten->SetActorHiddenInGame(true);
	}
}

// Called every frame
void AKitten::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AKitten::DecreaseKittenFood()
{
	if(KittenCurrentFood <= 0)
	{
		KittenCurrentFood = 0;
		GetWorld()->GetTimerManager().ClearTimer(FoodDecreaseTimerHandle);
		KittenMesh->SetVisibility(false);
		return;
	}

	if(UseItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Use Item True"));
		IncreaseKittenFood();
		return;
	}
	if(HasFoodCalStart && KittenCurrentFood >= 100)
	{
		
		GetWorld()->GetTimerManager().ClearTimer(FoodDecreaseTimerHandle);
		KittenCurrentFood = 100;
		UE_LOG(LogTemp, Warning, TEXT("%f"), KittenCurrentFood);
		return;
	}

	
	KittenCurrentFood = KittenCurrentFood - 0.1;

	if(KittenCurrentFood <= 0)
	{
		KittenCurrentFood = 0;
	}

	HasFoodCalStart = true;

	UE_LOG(LogTemp, Warning, TEXT("Food: %f"), KittenCurrentFood);
}

void AKitten::IncreaseKittenFood()
{
	if(KittenCurrentFood <= 0)
	{
		goOutKitten->SetActorHiddenInGame(false);
		return;
	}
	else
	{
		KittenMesh->SetVisibility(true);
		
	}

	
	GetWorld()->GetTimerManager().PauseTimer(FoodDecreaseTimerHandle);
	KittenCurrentFood = KittenCurrentFood + ItemPoint;
	UE_LOG(LogTemp, Warning, TEXT("%f"),KittenCurrentFood);
	UseItem = false;

	
	if(HasFoodCalStart && KittenCurrentFood >= 100)
	{
		KittenCurrentFood = 100;
		GetWorld()->GetTimerManager().ClearTimer(FoodDecreaseTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("%f"), KittenCurrentFood);
		return;
	}

	GetWorld()->GetTimerManager().UnPauseTimer(FoodDecreaseTimerHandle);
	
}

