// Fill out your copyright notice in the Description page of Project Settings.


#include "LSE/GoOutKitten.h"

#include "LSE/Kitten.h"

// Sets default values
AGoOutKitten::AGoOutKitten()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("Kitten"));
	GoOutKittenRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	GoOutKittenMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Kitten"));
	RootComponent = GoOutKittenRoot;
	GoOutKittenMesh->AttachToComponent(GoOutKittenRoot, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AGoOutKitten::BeginPlay()
{
	Super::BeginPlay();
	MyOwner = Cast<AKitten>(GetOwner());

		
}

// Called every frame
void AGoOutKitten::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

