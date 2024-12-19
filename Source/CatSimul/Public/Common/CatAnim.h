// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CatAnim.generated.h"

/**
 * 
 */
UCLASS()
class CATSIMUL_API UCatAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeBeginPlay() override;

	UPROPERTY()
	class ACatPlayer* player;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool bIsMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool bIsFalling;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	float GroundSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	float Vertical;
};
