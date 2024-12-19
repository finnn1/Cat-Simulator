// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CatPlayer.generated.h"

class UInputAction;
struct FInputActionValue;

UCLASS()
class CATSIMUL_API ACatPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACatPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
public:
	// 공용 사용 멤버 및 메서드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common")
	class USkeletalMeshComponent* SkeletalComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common")
	class UInputMappingContext* IMC_Cat;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common")
	UInputAction* IA_Turn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common")
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common")
	UInputAction* IA_Jump;

	float JumpHoldTime = 0; // 버튼 누름 시간
	FTimerHandle JumpTimerHandle;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void HandleJump(const FInputActionValue& Value);
	void StartJump();
	void StopJump();
	void UpdateJumpPower();
};
