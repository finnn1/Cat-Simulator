// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/CatPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ACatPlayer::ACatPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACatPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACatPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

// Called to bind functionality to input
void ACatPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Cat, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &ACatPlayer::Look);
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ACatPlayer::Move);
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Completed, this, &ACatPlayer::Move);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ACatPlayer::HandleJump);
	}
}

void ACatPlayer::Move(const FInputActionInstance& Instance)
{
	// 현재 트리거 상태 가져오기
	const ETriggerEvent TriggerEvent = Instance.GetTriggerEvent();
    
	// 현재 입력 값 가져오기
	float MovementSpeed = Instance.GetValue().Get<float>();

	if (TriggerEvent == ETriggerEvent::Triggered) // 트리거 상태 (키 입력 시작)
	{
		UE_LOG(LogTemp, Display, TEXT("Move Triggered"));
		bIsMove = true;

		// 속도 설정
		if (MovementSpeed > 0.f) // 전진
		{
			GetCharacterMovement()->MaxWalkSpeed = 600.f;
		}
		else if (MovementSpeed < 0.f) // 후진
		{
			GetCharacterMovement()->MaxWalkSpeed = 300.f;
		}

		// 이동 처리
		if (Controller != nullptr)
		{
			const FRotator ControlRotation = Controller->GetControlRotation();
			const FRotator YawRot(0, ControlRotation.Yaw, 0);

			const FVector ForwardDirection = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);

			AddMovementInput(ForwardDirection, MovementSpeed);
		}
	}
	else if (TriggerEvent == ETriggerEvent::Completed) // 입력 완료 상태 (키를 뗌)
	{
		UE_LOG(LogTemp, Display, TEXT("Move Completed"));
		bIsMove = false;
	}
}

void ACatPlayer::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	float DeltaTime = GetWorld()->GetDeltaSeconds();
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACatPlayer::HandleJump(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		// 키가 눌렸을 때 (Pressed)
		StartJump(); // 점프 시작
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StopJump"));
		// 키가 뗐을 때 (Released)
		StopJump(); // 점프 중단
	}
}

void ACatPlayer::StartJump()
{
	// 점프 시작
	JumpHoldTime = 0.0f;
	// 점프 속도 초기화
	GetCharacterMovement()->JumpZVelocity = 600.0f; // 기본값으로 복원
	// 버튼 누름 시간을 업데이트하는 타이머 시작
	GetWorldTimerManager().SetTimer(JumpTimerHandle, this, &ACatPlayer::UpdateJumpPower, GetWorld()->GetDeltaSeconds(), true);
}

void ACatPlayer::StopJump()
{
	// 타이머 정지
	GetWorldTimerManager().ClearTimer(JumpTimerHandle);

	// 점프 실행
	Jump();
}

void ACatPlayer::UpdateJumpPower()
{
	// 버튼 누름 시간 증가
	JumpHoldTime += GetWorld()->GetDeltaSeconds();
	// 점프 속도 동적 조정 (최대값 제한)
	float MaxHoldTime = 1.0f; // 최대 누를 수 있는 시간
	float BaseJumpZVelocity = 600.0f; // 기본 점프 속도
	float MaxJumpZVelocity = 1200.0f; // 최대 점프 속도

	float ScaledJumpZVelocity = FMath::Lerp(BaseJumpZVelocity, MaxJumpZVelocity, FMath::Clamp(JumpHoldTime / MaxHoldTime, 0.0f, 1.0f));
	UE_LOG(LogTemp, Warning, TEXT("%f"), ScaledJumpZVelocity);

	GetCharacterMovement()->JumpZVelocity = ScaledJumpZVelocity;
}

