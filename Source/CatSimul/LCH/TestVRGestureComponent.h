// VRGestureComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TestVRGestureComponent.generated.h"

UENUM(BlueprintType)
enum class EGestureType : uint8
{
    None,
    SwipeLeft,
    SwipeRight,
    SwipeUp,
    SwipeDown,
    Circle,
    Pinch,
    Grab,
    Custom
};

USTRUCT(BlueprintType)
struct FGestureData
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FVector> Points;
    
    UPROPERTY()
    float StartTime;
    
    UPROPERTY()
    float EndTime;
    
    UPROPERTY()
    float GestureSpeed;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGestureDetected, EGestureType, GestureType);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CATSIMUL_API UTestVRGestureComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    UTestVRGestureComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(BlueprintAssignable, Category = "VR|Gesture")
    FOnGestureDetected OnGestureDetected;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR|Gesture")
    float MinGestureDistance = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR|Gesture")
    float MaxGestureTime = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR|Gesture")
    float MinSwipeDistance = 30.0f;

public:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "VR|Gesture")
    void StartGestureCapture(const FVector& StartPosition);

    UFUNCTION(BlueprintCallable, Category = "VR|Gesture")
    void UpdateGestureCapture(const FVector& NewPosition);

    UFUNCTION(BlueprintCallable, Category = "VR|Gesture")
    void EndGestureCapture(const FVector& EndPosition);

private:
    FGestureData CurrentGesture;
    bool bIsCapturing;

    EGestureType RecognizeGesture(const FGestureData& GestureData);
    bool IsSwipeGesture(const FGestureData& GestureData, FVector& Direction) const;
    bool IsCircleGesture(const FGestureData& GestureData) const;
    float CalculateGestureSpeed(const FGestureData& GestureData) const;
};