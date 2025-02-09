// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoOutKitten.h"
#include "GameFramework/Actor.h"
#include "Kitten.generated.h"

UCLASS()
class CATSIMUL_API AKitten : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKitten();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KittenCurrentFood = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InterpSpeed;
	FTimerHandle FoodDecreaseTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DecreasePoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasFoodCalStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseItem = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ItemPoint;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* KittenMesh;
	
	UFUNCTION(BlueprintCallable)
	void DecreaseKittenFood();

	UFUNCTION(BlueprintCallable)
	void IncreaseKittenFood();
	
	UFUNCTION(BlueprintImplementableEvent)
	void setCompleteOpacity();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Out")
	TSubclassOf<AGoOutKitten> goOutCat;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Quest")
	bool QuestComp;
	
	AGoOutKitten* goOutKitten;

	UFUNCTION(BlueprintImplementableEvent)
	void OutAlert();
	
	UFUNCTION(BlueprintCallable)
	void SetKittenFoodTimer();

	
};
