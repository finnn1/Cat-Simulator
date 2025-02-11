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
	QuestComp = false;
	DecreasePoint = 0.03;

}

// Called when the game starts or when spawned
void AKitten::BeginPlay()
{
	Super::BeginPlay();

	if(goOutCat)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		//UE_LOG(LogTemp, Warning, TEXT("Kitten is alive"));
		//goOutKitten = GetWorld()->SpawnActor<AGoOutKitten>(goOutCat,FVector(1360,6480,0),FRotator(0, 90, 0), SpawnParams);		
		goOutKitten = GetWorld()->SpawnActor<AGoOutKitten>(goOutCat,FVector(2231.850535,8931.093114,5.585758),FRotator(0, 90, 0), SpawnParams);		
	}
	SetKittenFoodTimer();
	//goOutKitten->SetActorHiddenInGame(false);
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
		goOutKitten->SetActorHiddenInGame(false);
		OutAlert();
		return;
	}

	if(UseItem)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Use Item True"));
		IncreaseKittenFood();
		return;
	}
	if(HasFoodCalStart && KittenCurrentFood >= 100)
	{
		
		GetWorld()->GetTimerManager().ClearTimer(FoodDecreaseTimerHandle);
		KittenCurrentFood = 100;
		QuestComp = true;
		return;
	}

	
	KittenCurrentFood = KittenCurrentFood - DecreasePoint;
//	KittenCurrentFood = KittenCurrentFood - 0.03;

	//KittenCurrentFood = KittenCurrentFood - 5;

	if(KittenCurrentFood <= 0)
	{
		KittenCurrentFood = 0;
		KittenMesh->SetVisibility(false);
		goOutKitten->SetActorHiddenInGame(false);
		OutAlert();
	}
	
	HasFoodCalStart = true;

	//UE_LOG(LogTemp, Warning, TEXT("Food: %f"), KittenCurrentFood);
}

void AKitten::IncreaseKittenFood()
{

	GetWorld()->GetTimerManager().PauseTimer(FoodDecreaseTimerHandle);
	KittenCurrentFood = KittenCurrentFood + ItemPoint;
	
	UseItem = false;

	if(HasFoodCalStart && KittenCurrentFood >= 100)
	{
		KittenCurrentFood = 100;
		QuestComp = true;
		GetWorld()->GetTimerManager().ClearTimer(FoodDecreaseTimerHandle);

		//UE_LOG(LogTemp, Warning, TEXT("%f"), KittenCurrentFood);
		return;
	}

	GetWorld()->GetTimerManager().UnPauseTimer(FoodDecreaseTimerHandle);
	
}

void AKitten::SetKittenFoodTimer()
{
	GetWorld()->GetTimerManager().SetTimer(
	FoodDecreaseTimerHandle,
	this,
	&AKitten::DecreaseKittenFood,
	0.2f,
	true
	);
}

