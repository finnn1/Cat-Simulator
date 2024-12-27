// VRGestureComponent.cpp
#include "TestVRGestureComponent.h"

UTestVRGestureComponent::UTestVRGestureComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    bIsCapturing = false;
}

void UTestVRGestureComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UTestVRGestureComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsCapturing)
    {
        // 제스처 시간 초과 체크
        // 설정된 최대 시간을 초과하면 제스처 캡처 종료
        if (GetWorld()->GetTimeSeconds() - CurrentGesture.StartTime > MaxGestureTime)
        {
            bIsCapturing = false;
        }
    }
}

// 제스처 캡처 시작
void UTestVRGestureComponent::StartGestureCapture(const FVector& StartPosition)
{
    bIsCapturing = true;
    CurrentGesture.Points.Empty();                     // 포인트 배열 초기화
    CurrentGesture.Points.Add(StartPosition);          // 시작 위치 저장
    CurrentGesture.StartTime = GetWorld()->GetTimeSeconds();  // 시작 시간 기록
}

// 제스처 포인트 업데이트
void UTestVRGestureComponent::UpdateGestureCapture(const FVector& NewPosition)
{
    if (bIsCapturing)
    {
        CurrentGesture.Points.Add(NewPosition);  // 새로운 위치 포인트 추가
    }
}

// 제스처 캡처 종료
void UTestVRGestureComponent::EndGestureCapture(const FVector& EndPosition)
{
    if (bIsCapturing)
    {
        CurrentGesture.Points.Add(EndPosition);
        CurrentGesture.EndTime = GetWorld()->GetTimeSeconds();
        CurrentGesture.GestureSpeed = CalculateGestureSpeed(CurrentGesture);

        EGestureType DetectedGesture = RecognizeGesture(CurrentGesture);
        if (DetectedGesture != EGestureType::None)
        {
            OnGestureDetected.Broadcast(DetectedGesture);
        }

        bIsCapturing = false;
    }
}

EGestureType UTestVRGestureComponent::RecognizeGesture(const FGestureData& GestureData)
{
    if (GestureData.Points.Num() < 2)
        return EGestureType::None;

    FVector Direction;
    if (IsSwipeGesture(GestureData, Direction))
    {
        if (FMath::Abs(Direction.X) > FMath::Abs(Direction.Y))
        {
            return Direction.X > 0 ? EGestureType::SwipeRight : EGestureType::SwipeLeft;
        }
        else
        {
            return Direction.Z > 0 ? EGestureType::SwipeUp : EGestureType::SwipeDown;
        }
    }
    else if (IsCircleGesture(GestureData))
    {
        return EGestureType::Circle;
    }

    return EGestureType::None;
}

bool UTestVRGestureComponent::IsSwipeGesture(const FGestureData& GestureData, FVector& Direction) const
{
    if (GestureData.Points.Num() < 2)
        return false;

    FVector Start = GestureData.Points[0];
    FVector End = GestureData.Points.Last();
    Direction = End - Start;
    
    float Distance = Direction.Size();
    if (Distance < MinSwipeDistance)
        return false;

    Direction.Normalize();
    return true;
}

bool UTestVRGestureComponent::IsCircleGesture(const FGestureData& GestureData) const
{
    if (GestureData.Points.Num() < 8)
        return false;

    // Simple circle detection
    FVector Center = FVector::ZeroVector;
    for (const FVector& Point : GestureData.Points)
    {
        Center += Point;
    }
    Center /= GestureData.Points.Num();

    float AverageRadius = 0.0f;
    for (const FVector& Point : GestureData.Points)
    {
        AverageRadius += (Point - Center).Size();
    }
    AverageRadius /= GestureData.Points.Num();

    float RadiusVariance = 0.0f;
    for (const FVector& Point : GestureData.Points)
    {
        float CurrentRadius = (Point - Center).Size();
        RadiusVariance += FMath::Abs(CurrentRadius - AverageRadius);
    }
    RadiusVariance /= GestureData.Points.Num();

    return RadiusVariance < MinGestureDistance * 0.5f;
}

float UTestVRGestureComponent::CalculateGestureSpeed(const FGestureData& GestureData) const
{
    if (GestureData.Points.Num() < 2)
        return 0.0f;

    float TotalDistance = 0.0f;
    for (int32 i = 1; i < GestureData.Points.Num(); ++i)
    {
        TotalDistance += FVector::Distance(GestureData.Points[i], GestureData.Points[i-1]);
    }

    float TimeSpent = GestureData.EndTime - GestureData.StartTime;
    return TimeSpent > 0.0f ? TotalDistance / TimeSpent : 0.0f;
}