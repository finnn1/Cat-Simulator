#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "TestVRHandMoveCharacter.generated.h"

UCLASS()
class CATSIMUL_API ATestVRHandMoveCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ATestVRHandMoveCharacter();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // VR 컴포넌트들
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
    UCameraComponent* VRCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
    class UMotionControllerComponent* LeftHandController;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
    class UMotionControllerComponent* RightHandController;

    // 충돌체들
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
    class USphereComponent* HeadCollider;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
    class UCapsuleComponent* BodyCollider;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
    class USceneComponent* LeftHandFollower;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR")
    class USceneComponent* RightHandFollower;

    // 이동 관련 파라미터
    UPROPERTY(EditAnywhere, Category = "Gorilla Movement")
    float MaxArmLength = 150.0f;  // 1.5m in Unreal units

    UPROPERTY(EditAnywhere, Category = "Gorilla Movement")
    float UnStickDistance = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Gorilla Movement")
    float VelocityLimit = 1000.0f;

    UPROPERTY(EditAnywhere, Category = "Gorilla Movement")
    float MaxJumpSpeed = 2000.0f;

    UPROPERTY(EditAnywhere, Category = "Gorilla Movement")
    float JumpMultiplier = 1.5f;

    UPROPERTY(EditAnywhere, Category = "Gorilla Movement")
    float MinimumRaycastDistance = 5.0f;

    UPROPERTY(EditAnywhere, Category = "Gorilla Movement")
    float DefaultSlideFactor = 0.03f;

    UPROPERTY(EditAnywhere, Category = "Gorilla Movement")
    float DefaultPrecision = 0.995f;

    // 속도 이력 관리
    UPROPERTY(EditAnywhere, Category = "Gorilla Movement")
    int32 VelocityHistorySize = 30;

    // 핸드 오프셋
    UPROPERTY(EditAnywhere, Category = "Gorilla Movement")
    FVector LeftHandOffset;

    UPROPERTY(EditAnywhere, Category = "Gorilla Movement")
    FVector RightHandOffset;
    
    void UpdateHandPositions();
    void ProcessJumpVelocity();
    void CheckHandSticking();

private:
    // 상태 변수들
    FVector LastLeftHandPosition;
    FVector LastRightHandPosition;
    FVector LastHeadPosition;
    
    TArray<FVector> VelocityHistory;
    int32 VelocityIndex;
    FVector CurrentVelocity;
    FVector DenormalizedVelocityAverage;
    bool bWasLeftHandTouching;
    bool bWasRightHandTouching;
    FVector LastPosition;
    bool bDisableMovement;

    // 핸드 위치 계산 함수들
    FVector GetCurrentLeftHandPosition() const;
    FVector GetCurrentRightHandPosition() const;
    FVector PositionWithOffset(const FTransform& Transform, const FVector& Offset) const;

    // 충돌 처리 함수들
    bool IterativeCollisionSphereCast(const FVector& StartPosition, float SphereRadius, 
        const FVector& MovementVector, float Precision, FVector& EndPosition, bool bSingleHand);
    
    bool CollisionsSphereCast(const FVector& StartPosition, float SphereRadius,
        const FVector& MovementVector, float Precision, FVector& FinalPosition, FHitResult& HitInfo);

    // 속도 관련 함수들
    void StoreVelocities(float DeltaTime);

public:
    // 공개 함수들
    UFUNCTION(BlueprintCallable, Category = "Gorilla Movement")
    bool IsHandTouching(bool bForLeftHand) const;

    UFUNCTION(BlueprintCallable, Category = "Gorilla Movement")
    void Turn(float Degrees);

    UFUNCTION(BlueprintCallable, Category = "Gorilla Movement")
    void SetDisableMovement(bool bDisable) { bDisableMovement = bDisable; }
};