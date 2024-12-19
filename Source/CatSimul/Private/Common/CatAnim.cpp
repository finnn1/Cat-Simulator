// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/CatAnim.h"

#include "Common/CatPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCatAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	player = Cast<ACatPlayer>(TryGetPawnOwner());
}

void UCatAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!player) return;

	// 캐릭터의 속도를 가져옴
	FVector Velocity = player->GetVelocity();
        
	// XY 평면의 속도 크기를 계산
	FVector Velocity2D = FVector(Velocity.X, Velocity.Y, 0.0f);
	GroundSpeed = Velocity2D.Size();
		
	// 움직임 상태 업데이트 (GroundSpeed가 특정 값 이상일 때만 True)
	bIsMove = player->bIsMove; // 임계값은 3.0f로 설정

	// Transform을 이용하여 월드 속도를 로컬 좌표로 변환
	FVector LocalVelocity = player->GetActorTransform().InverseTransformVector(Velocity);

	Vertical = LocalVelocity.X;

	SpeedZ = Velocity.Z;

	bIsFalling = player->GetCharacterMovement()->IsFalling();

	
}

