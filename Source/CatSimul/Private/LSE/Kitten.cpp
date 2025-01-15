// Fill out your copyright notice in the Description page of Project Settings.


#include "LSE/Kitten.h"

// Sets default values
AKitten::AKitten()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("Kitten"));

	///RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	KittenMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Kitten"));
	//MovableMesh->SetupAttachment(RootComponent);
	RootComponent = KittenMesh;

	InterpSpeed = 0.1f;
	

}

// Called when the game starts or when spawned
void AKitten::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(
		FoodDecreaseTimerHandle,
		this,
		&AKitten::DecreaseKittenFood,
		0.05f,
		true
	);
}

// Called every frame
void AKitten::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AKitten::DecreaseKittenFood()
{
	
	KittenCurrentFood = FMath::FInterpTo(KittenCurrentFood, KittenTargetFood, GetWorld()->GetDeltaSeconds(), InterpSpeed);
	if(FMath::IsNearlyEqual(KittenCurrentFood, KittenTargetFood, 0.01f))
	{
		KittenCurrentFood = KittenTargetFood;
	}
	
	KittenTargetFood = KittenTargetFood - 0.5;
	if(KittenTargetFood <= 0)
	{
		KittenTargetFood = 0;
	}
	UE_LOG(LogTemp, Warning, TEXT("Food: %f"), KittenCurrentFood);
	return;
}

