#include "TestVRHandMoveCharacter.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ATestVRHandMoveCharacter::ATestVRHandMoveCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // 컴포넌트 생성 및 설정
    VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
    VRCamera->SetupAttachment(GetRootComponent());
    VRCamera->bLockToHmd = true;

    HeadCollider = CreateDefaultSubobject<USphereComponent>(TEXT("HeadCollider"));
    HeadCollider->SetupAttachment(VRCamera);
    HeadCollider->SetSphereRadius(15.0f);  // 15cm radius

    BodyCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCollider"));
    BodyCollider->SetupAttachment(GetRootComponent());
    BodyCollider->SetCapsuleHalfHeight(50.0f);
    BodyCollider->SetCapsuleRadius(20.0f);

    LeftHandController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHandController"));
    LeftHandController->SetupAttachment(GetRootComponent());
    LeftHandController->SetTrackingSource(EControllerHand::Left);

    RightHandController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHandController"));
    RightHandController->SetupAttachment(GetRootComponent());
    RightHandController->SetTrackingSource(EControllerHand::Right);

    LeftHandFollower = CreateDefaultSubobject<USceneComponent>(TEXT("LeftHandFollower"));
    LeftHandFollower->SetupAttachment(GetRootComponent());

    RightHandFollower = CreateDefaultSubobject<USceneComponent>(TEXT("RightHandFollower"));
    RightHandFollower->SetupAttachment(GetRootComponent());

    // 초기값 설정
    VelocityHistory.Init(FVector::ZeroVector, VelocityHistorySize);
    bDisableMovement = false;
}

void ATestVRHandMoveCharacter::BeginPlay()
{
    Super::BeginPlay();

    // 초기 위치 설정
    LastLeftHandPosition = LeftHandFollower->GetComponentLocation();
    LastRightHandPosition = RightHandFollower->GetComponentLocation();
    LastHeadPosition = HeadCollider->GetComponentLocation();
    VelocityIndex = 0;
    LastPosition = GetActorLocation();
}

FVector ATestVRHandMoveCharacter::GetCurrentLeftHandPosition() const
{
    FVector HandPosition = PositionWithOffset(LeftHandController->GetComponentTransform(), LeftHandOffset);
    FVector HeadToHand = HandPosition - HeadCollider->GetComponentLocation();
    
    if (HeadToHand.Size() < MaxArmLength)
    {
        return HandPosition;
    }
    return HeadCollider->GetComponentLocation() + HeadToHand.GetSafeNormal() * MaxArmLength;
}

FVector ATestVRHandMoveCharacter::GetCurrentRightHandPosition() const
{
    FVector HandPosition = PositionWithOffset(RightHandController->GetComponentTransform(), RightHandOffset);
    FVector HeadToHand = HandPosition - HeadCollider->GetComponentLocation();
    
    if (HeadToHand.Size() < MaxArmLength)
    {
        return HandPosition;
    }
    return HeadCollider->GetComponentLocation() + HeadToHand.GetSafeNormal() * MaxArmLength;
}

FVector ATestVRHandMoveCharacter::PositionWithOffset(const FTransform& Transform, const FVector& Offset) const
{
    return Transform.GetLocation() + Transform.GetRotation().RotateVector(Offset);
}

void ATestVRHandMoveCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    bool bLeftHandColliding = false;
    bool bRightHandColliding = false;
    FVector FinalPosition;
    FVector RigidBodyMovement = FVector::ZeroVector;
    FVector FirstIterationLeftHand = FVector::ZeroVector;
    FVector FirstIterationRightHand = FVector::ZeroVector;
    FHitResult HitInfo;

    // 바디 콜라이더 회전 업데이트
    FRotator NewRotation = FRotator(0.0f, HeadCollider->GetComponentRotation().Yaw, 0.0f);
    BodyCollider->SetWorldRotation(NewRotation);

    // 왼손 처리
    FVector DistanceTraveled = GetCurrentLeftHandPosition() - LastLeftHandPosition;
    DistanceTraveled += FVector(0, 0, -980.0f) * DeltaTime * DeltaTime; // 중력 적용

    if (IterativeCollisionSphereCast(LastLeftHandPosition, MinimumRaycastDistance, 
        DistanceTraveled, DefaultPrecision, FinalPosition, true))
    {
        if (bWasLeftHandTouching)
        {
            FirstIterationLeftHand = LastLeftHandPosition - GetCurrentLeftHandPosition();
        }
        else
        {
            FirstIterationLeftHand = FinalPosition - GetCurrentLeftHandPosition();
        }

        bLeftHandColliding = true;
    }

    // 오른손 처리
    DistanceTraveled = GetCurrentRightHandPosition() - LastRightHandPosition;
    DistanceTraveled += FVector(0, 0, -980.0f) * DeltaTime * DeltaTime; // 중력 적용

    if (IterativeCollisionSphereCast(LastRightHandPosition, MinimumRaycastDistance,
        DistanceTraveled, DefaultPrecision, FinalPosition, true))
    {
        if (bWasRightHandTouching)
        {
            FirstIterationRightHand = LastRightHandPosition - GetCurrentRightHandPosition();
        }
        else
        {
            FirstIterationRightHand = FinalPosition - GetCurrentRightHandPosition();
        }

        bRightHandColliding = true;
    }

    // 손 움직임 조합
    if ((bLeftHandColliding || bWasLeftHandTouching) && (bRightHandColliding || bWasRightHandTouching))
    {
        RigidBodyMovement = (FirstIterationLeftHand + FirstIterationRightHand) * 0.5f;
    }
    else
    {
        RigidBodyMovement = FirstIterationLeftHand + FirstIterationRightHand;
    }

    // 머리 충돌 체크
    if (!RigidBodyMovement.IsZero())
    {
        FVector DesiredHeadMovement = HeadCollider->GetComponentLocation() + RigidBodyMovement - LastHeadPosition;
        if (IterativeCollisionSphereCast(LastHeadPosition, HeadCollider->GetUnscaledSphereRadius(),
            DesiredHeadMovement, DefaultPrecision, FinalPosition, false))
        {
            RigidBodyMovement = FinalPosition - LastHeadPosition;

            // 추가 안전 체크
            FHitResult HeadHit;
            if (GetWorld()->LineTraceSingleByChannel(HeadHit, LastHeadPosition, 
                HeadCollider->GetComponentLocation() - LastHeadPosition + RigidBodyMovement,
                ECC_Visibility))
            {
                RigidBodyMovement = LastHeadPosition - HeadCollider->GetComponentLocation();
            }
        }
    }

    // 위치 업데이트
    if (!RigidBodyMovement.IsZero())
    {
        SetActorLocation(GetActorLocation() + RigidBodyMovement);
    }

    LastHeadPosition = HeadCollider->GetComponentLocation();

    // 손 위치 최종 업데이트
    UpdateHandPositions();
    
    // 속도 저장 및 점프 처리
    StoreVelocities(DeltaTime);
    
    if ((bRightHandColliding || bLeftHandColliding) && !bDisableMovement)
    {
        ProcessJumpVelocity();
    }

    // 손 고정 해제 체크
    CheckHandSticking();

    // 손 팔로워 위치 업데이트
    LeftHandFollower->SetWorldLocation(LastLeftHandPosition);
    RightHandFollower->SetWorldLocation(LastRightHandPosition);

    bWasLeftHandTouching = bLeftHandColliding;
    bWasRightHandTouching = bRightHandColliding;
}

void ATestVRHandMoveCharacter::UpdateHandPositions()
{
    FVector DistanceTraveled = GetCurrentLeftHandPosition() - LastLeftHandPosition;
    FVector FinalPosition;

if (IterativeCollisionSphereCast(LastLeftHandPosition, MinimumRaycastDistance, DistanceTraveled,
        DefaultPrecision, FinalPosition, !(bWasLeftHandTouching && bWasRightHandTouching)))
    {
        LastLeftHandPosition = FinalPosition;
        bWasLeftHandTouching = true;
    }
    else
    {
        LastLeftHandPosition = GetCurrentLeftHandPosition();
    }

    DistanceTraveled = GetCurrentRightHandPosition() - LastRightHandPosition;
    if (IterativeCollisionSphereCast(LastRightHandPosition, MinimumRaycastDistance, DistanceTraveled,
        DefaultPrecision, FinalPosition, !(bWasLeftHandTouching && bWasRightHandTouching)))
    {
        LastRightHandPosition = FinalPosition;
        bWasRightHandTouching = true;
    }
    else
    {
        LastRightHandPosition = GetCurrentRightHandPosition();
    }
}

void ATestVRHandMoveCharacter::ProcessJumpVelocity()
{
    if (DenormalizedVelocityAverage.Size() > VelocityLimit)
    {
        FVector JumpVelocity;
        if (DenormalizedVelocityAverage.Size() * JumpMultiplier > MaxJumpSpeed)
        {
            JumpVelocity = DenormalizedVelocityAverage.GetSafeNormal() * MaxJumpSpeed;
        }
        else
        {
            JumpVelocity = DenormalizedVelocityAverage * JumpMultiplier;
        }
        
        GetCharacterMovement()->Velocity = JumpVelocity;
    }
}

void ATestVRHandMoveCharacter::CheckHandSticking()
{
    FHitResult HitInfo;

    // 왼손 고정 해제 체크
    if (bWasLeftHandTouching && (GetCurrentLeftHandPosition() - LastLeftHandPosition).Size() > UnStickDistance)
    {
        FVector HeadToHand = GetCurrentLeftHandPosition() - HeadCollider->GetComponentLocation();
        if (!UKismetSystemLibrary::SphereTraceSingle(
            GetWorld(),
            HeadCollider->GetComponentLocation(),
            GetCurrentLeftHandPosition(),
            MinimumRaycastDistance * DefaultPrecision,
            UEngineTypes::ConvertToTraceType(ECC_Visibility),
            false,
            TArray<AActor*>(),
            EDrawDebugTrace::None,
            HitInfo,
            true))
        {
            LastLeftHandPosition = GetCurrentLeftHandPosition();
            bWasLeftHandTouching = false;
        }
    }

    // 오른손 고정 해제 체크
    if (bWasRightHandTouching && (GetCurrentRightHandPosition() - LastRightHandPosition).Size() > UnStickDistance)
    {
        FVector HeadToHand = GetCurrentRightHandPosition() - HeadCollider->GetComponentLocation();
        if (!UKismetSystemLibrary::SphereTraceSingle(
            GetWorld(),
            HeadCollider->GetComponentLocation(),
            GetCurrentRightHandPosition(),
            MinimumRaycastDistance * DefaultPrecision,
            UEngineTypes::ConvertToTraceType(ECC_Visibility),
            false,
            TArray<AActor*>(),
            EDrawDebugTrace::None,
            HitInfo,
            true))
        {
            LastRightHandPosition = GetCurrentRightHandPosition();
            bWasRightHandTouching = false;
        }
    }
}

bool ATestVRHandMoveCharacter::IterativeCollisionSphereCast(const FVector& StartPosition, float SphereRadius,
    const FVector& MovementVector, float Precision, FVector& EndPosition, bool bSingleHand)
{
    FHitResult HitInfo;
    FVector MovementToProjectedAboveCollisionPlane;
    float SlipPercentage;

    // 첫 번째 구체 캐스트
    if (CollisionsSphereCast(StartPosition, SphereRadius * Precision, MovementVector,
        Precision, EndPosition, HitInfo))
    {
        FVector FirstPosition = EndPosition;
        SlipPercentage = bSingleHand ? 0.001f : DefaultSlideFactor;

        // 표면 위에서의 미끄러짐 계산
        MovementToProjectedAboveCollisionPlane = FVector::VectorPlaneProject(
            StartPosition + MovementVector - FirstPosition,
            HitInfo.Normal) * SlipPercentage;

        if (CollisionsSphereCast(EndPosition, SphereRadius, MovementToProjectedAboveCollisionPlane,
            Precision * Precision, EndPosition, HitInfo))
        {
            return true;
        }
        else if (CollisionsSphereCast(MovementToProjectedAboveCollisionPlane + FirstPosition,
            SphereRadius, StartPosition + MovementVector - (MovementToProjectedAboveCollisionPlane + FirstPosition),
            Precision * Precision * Precision, EndPosition, HitInfo))
        {
            return true;
        }
        else
        {
            EndPosition = FirstPosition;
            return true;
        }
    }
    else if (CollisionsSphereCast(StartPosition, SphereRadius * Precision * 0.66f,
        MovementVector.GetSafeNormal() * (MovementVector.Size() + SphereRadius * Precision * 0.34f),
        Precision * 0.66f, EndPosition, HitInfo))
    {
        EndPosition = StartPosition;
        return true;
    }

    EndPosition = FVector::ZeroVector;
    return false;
}

bool ATestVRHandMoveCharacter::CollisionsSphereCast(const FVector& StartPosition, float SphereRadius,
    const FVector& MovementVector, float Precision, FVector& FinalPosition, FHitResult& HitInfo)
{
    FCollisionQueryParams QueryParams;
    QueryParams.bTraceComplex = true;
    QueryParams.AddIgnoredActor(this);

    // 초기 구체 스윕
    if (GetWorld()->SweepSingleByChannel(HitInfo, StartPosition, StartPosition + MovementVector,
        FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(SphereRadius * Precision), QueryParams))
    {
        // 충돌 지점으로부터 구체 반지름만큼 떨어진 위치 계산
        FinalPosition = HitInfo.Location + HitInfo.Normal * SphereRadius;

        // 추가 안전 체크
        FHitResult InnerHit;
        FVector DirectionToFinal = FinalPosition - StartPosition;
        float DistanceToFinal = DirectionToFinal.Size();

        if (GetWorld()->SweepSingleByChannel(InnerHit, StartPosition, FinalPosition,
            FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(SphereRadius * Precision * Precision),
            QueryParams))
        {
            FinalPosition = StartPosition + DirectionToFinal.GetSafeNormal() *
                FMath::Max(0.0f, HitInfo.Distance - SphereRadius * (1.0f - Precision * Precision));
            HitInfo = InnerHit;
        }
        else if (GetWorld()->LineTraceSingleByChannel(InnerHit, StartPosition, FinalPosition,
            ECC_Visibility, QueryParams))
        {
            FinalPosition = StartPosition;
            HitInfo = InnerHit;
            return true;
        }
        return true;
    }
    else if (GetWorld()->LineTraceSingleByChannel(HitInfo, StartPosition, StartPosition + MovementVector,
        ECC_Visibility, QueryParams))
    {
        FinalPosition = StartPosition;
        return true;
    }

    FinalPosition = FVector::ZeroVector;
    return false;
}

void ATestVRHandMoveCharacter::StoreVelocities(float DeltaTime)
{
    if (DeltaTime <= 0.0f) return;

    VelocityIndex = (VelocityIndex + 1) % VelocityHistorySize;
    FVector OldestVelocity = VelocityHistory[VelocityIndex];
    CurrentVelocity = (GetActorLocation() - LastPosition) / DeltaTime;
    DenormalizedVelocityAverage += (CurrentVelocity - OldestVelocity) / VelocityHistorySize;
    VelocityHistory[VelocityIndex] = CurrentVelocity;
    LastPosition = GetActorLocation();
}

void ATestVRHandMoveCharacter::Turn(float Degrees)
{
    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += Degrees;
    SetActorRotation(NewRotation);

    // 속도 벡터 회전
    FQuat Rotation = FQuat(FRotator(0, Degrees, 0));
    DenormalizedVelocityAverage = Rotation.RotateVector(DenormalizedVelocityAverage);
    
    // 속도 이력 회전
    for (FVector& Velocity : VelocityHistory)
    {
        Velocity = Rotation.RotateVector(Velocity);
    }
}

bool ATestVRHandMoveCharacter::IsHandTouching(bool bForLeftHand) const
{
    return bForLeftHand ? bWasLeftHandTouching : bWasRightHandTouching;
}