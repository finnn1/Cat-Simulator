// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LCU/Inventory/UItem.h"
#include "UFoodItem.generated.h"

UENUM(Blueprintable,BlueprintType)
enum class EFoodOption : uint8
{
	Cook UMETA(DisplayName="Cook"),
	Cut UMETA(DisplayName="Cut")
};

USTRUCT(Blueprintable, BlueprintType)
struct FFoodAttribute
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EFoodOption> FoodOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FullFood;

	//UFUNCTION(BlueprintCallable)
	bool HasFoodOption(const EFoodOption& option);
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
	FFoodAttribute Food;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AUFoodItem> SilcedFood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SliceNum;
	
};
