// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LCU/Inventory/UItem.h"
#include "UFoodItem.generated.h"

UENUM()
enum class EFoodProperty : uint8
{
	Cook UMETA(DisplayName = " Cook"),
	Cut UMETA(DisplayName = " Cut"),
	Respawn UMETA(DisplayName = "Respawn")
};

USTRUCT(Blueprintable, BlueprintType)
struct FFoodAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EFoodProperty> FoodProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FullFood;

	bool HasFoodProperty(EFoodProperty property);
};

UCLASS()
class CATSIMUL_API AUFoodItem : public AUItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUFoodItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFoodAttributes FoodAttributes;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AUFoodItem> SilcedFood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SliceNum;
	
};
