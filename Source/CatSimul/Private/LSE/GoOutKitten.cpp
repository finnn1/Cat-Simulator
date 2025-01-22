// Fill out your copyright notice in the Description page of Project Settings.


#include "LSE/GoOutKitten.h"

// Sets default values
AGoOutKitten::AGoOutKitten()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("Kitten"));
	GoOutKittenMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Kitten"));
	RootComponent = GoOutKittenMesh;
}

// Called when the game starts or when spawned
void AGoOutKitten::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoOutKitten::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

