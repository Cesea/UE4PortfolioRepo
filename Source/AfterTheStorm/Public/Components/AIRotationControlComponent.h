// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIRotationControlComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERTHESTORM_API UAIRotationControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIRotationControlComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRotateCompletedEvent);

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category="AIRotation")
	FRotateCompletedEvent OnRotateCompleted;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "AIRotation")
	void RotateBy(float Angle, float AngleDeviation = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "AIRotation")
	void StartRotateToTarget(AActor* NewTarget, float NewGap = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "AIRotation")
	void StopRotateToTarget();

protected : 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIRotation")
	class AActor* Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIRotation")
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIRotation")
	float SimilarityGap{ 0.3f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIRotation")
	float DefaultSimilarityGap{ 0.3f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIRotation")
	float RotationSpeed{9.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIRotation")
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIRotation")
	uint32 bIsRotating : 1;

};
