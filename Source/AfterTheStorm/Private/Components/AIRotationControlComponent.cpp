// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/AIRotationControlComponent.h"

#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UAIRotationControlComponent::UAIRotationControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.SetTickFunctionEnable(false);

	// ...
}


// Called when the game starts
void UAIRotationControlComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UAIRotationControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsRotating)
	{
		FRotator LookAtRotator;
		FRotator TargetRotator;
		if (Target != nullptr)
		{
			FVector Location = GetOwner()->GetActorLocation();
			FVector ZAdjustedTargetLocation = Target->GetActorLocation();
			ZAdjustedTargetLocation.Z = Location.Z;

			LookAtRotator = UKismetMathLibrary::FindLookAtRotation(Location, ZAdjustedTargetLocation);
			TargetRotator = UKismetMathLibrary::RInterpTo(GetOwner()->GetActorRotation(), LookAtRotator, DeltaTime, RotationSpeed);
		}
		else
		{
			FVector Location = GetOwner()->GetActorLocation();
			FVector ZAdjustedTargetLocation = TargetLocation;
			ZAdjustedTargetLocation.Z = Location.Z;

			LookAtRotator = UKismetMathLibrary::FindLookAtRotation(Location, ZAdjustedTargetLocation);
			TargetRotator = UKismetMathLibrary::RInterpTo(GetOwner()->GetActorRotation(), LookAtRotator, DeltaTime, RotationSpeed);
		}
		Direction = LookAtRotator.Yaw < TargetRotator.Yaw ? -1 : 1;
		GetOwner()->SetActorRotation(TargetRotator, ETeleportType::None);

		if (UKismetMathLibrary::EqualEqual_RotatorRotator(GetOwner()->GetActorRotation(), LookAtRotator, SimilarityGap))
		{
			OnRotateCompleted.Broadcast();
			StopRotateToTarget();
		}
	}
}

void UAIRotationControlComponent::RotateBy(float Angle, float AngleDeviation)
{
	PrimaryComponentTick.SetTickFunctionEnable(true);

	float RandomAngle = FMath::RandRange(-AngleDeviation, AngleDeviation);

	FVector RotatedVector = UKismetMathLibrary::RotateAngleAxis(GetOwner()->GetActorForwardVector(), Angle + RandomAngle, FVector::UpVector);

	TargetLocation = GetOwner()->GetActorLocation() + RotatedVector * 100.0f;
	bIsRotating = true;
}

void UAIRotationControlComponent::StartRotateToTarget(AActor* NewTarget, float NewGap)
{
	if (!FMath::IsNearlyZero(NewGap))
	{
		SimilarityGap = NewGap;
	}

	PrimaryComponentTick.SetTickFunctionEnable(true);
	Target = NewTarget;
	bIsRotating = true;
}

void UAIRotationControlComponent::StopRotateToTarget()
{
	SimilarityGap = DefaultSimilarityGap;
	PrimaryComponentTick.SetTickFunctionEnable(false);
	Target = nullptr;
	bIsRotating = false;
	OnRotateCompleted.Clear();
}

