// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UItem.generated.h"

UCLASS(BlueprintType, Blueprintable)
class CATSIMUL_API AUItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Get, Set
	bool GetCanRespawn() {return bCanRespawn;};
	void SetCanRespawn(bool can) {bCanRespawn = can;};

	// Get, Set end
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UUItemDataComponent* ItemDataComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanRespawn = true;
};
