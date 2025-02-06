// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemManager.generated.h"

class AUItem;

USTRUCT(BlueprintType)
struct FItemRespawnData
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Location; // 아이템 위치

	UPROPERTY()
	TSubclassOf<AUItem> ItemClass; // 아이템 클래스

	UPROPERTY()
	float RespawnTime; // 리스폰 타이머
};

UCLASS()
class CATSIMUL_API AItemManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	// 아이템 줍기 함수
	UFUNCTION(BlueprintCallable)
	void CheckRespawnItem(AUItem* Item);

private:
	// 리스폰 데이터를 저장할 맵
	TMap<FName, FItemRespawnData> RespawnData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UDataTable* ItemDataTable;

	// 리스폰 처리 함수
	void RespawnItem(FName ItemID);
};
