// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/RotationControlComponent.h"

#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
URotationControlComponent::URotationControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PrimaryComponentTick.SetTickFunctionEnable(false);

	RotationSpeed = 2.0f;
}


// Called when the game starts
void URotationControlComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URotationControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ElapsedTime += DeltaTime;

	if (ElapsedTime > EstimatedTime)
	{
		ElapsedTime = 0.0f;
		TargetActor = nullptr;
		PrimaryComponentTick.SetTickFunctionEnable(false);
	}

	//FQuat TargetQuat;
	if (TargetActor != nullptr)
	{
		FVector ActorForward = GetOwner()->GetActorForwardVector();


		FVector ActorLocation = GetOwner()->GetActorLocation();
		FVector TargetZAdjusted = TargetActor->GetActorLocation();
		TargetZAdjusted.Z = ActorLocation.Z;

		FVector Direction = (TargetZAdjusted - ActorLocation).GetSafeNormal();

		FQuat TargetQuat = FQuat::Slerp(GetOwner()->GetActorForwardVector().ToOrientationQuat(), Direction.ToOrientationQuat(), ElapsedTime/EstimatedTime);
		GetOwner()->SetActorRotation(TargetQuat);
	}
	else
	{
		FQuat TargetQuat = FQuat::Slerp(GetOwner()->GetActorForwardVector().ToOrientationQuat(), TargetDirection.ToOrientationQuat(), ElapsedTime/EstimatedTime);
		GetOwner()->SetActorRotation(TargetQuat);
	}
}



void URotationControlComponent::RotateTowardsDirection(FVector Direction)
{
	ElapsedTime = 0.0001f;

	TargetDirection = Direction;

	FVector ActorForward = GetOwner()->GetActorForwardVector();

	float DotBetween = 1.0f - (FVector::DotProduct(Direction, ActorForward) * 0.5f + 0.5f);
	if (!FMath::IsNearlyZero(DotBetween))
	{
		PrimaryComponentTick.SetTickFunctionEnable(true);
	}
	EstimatedTime =  DotBetween / RotationSpeed;
}

void URotationControlComponent::RotateTowardsActor(AActor* NewTarget)
{
	ElapsedTime = 0.0001f;

	this->TargetActor = NewTarget;

	FVector ActorForward = GetOwner()->GetActorForwardVector();
	FVector Direction = NewTarget->GetActorLocation() - GetOwner()->GetActorLocation();
	Direction.Normalize();

	float DotBetween = 1.0f - (FVector::DotProduct(Direction, ActorForward) * 0.5f + 0.5f);
	if (!FMath::IsNearlyZero(DotBetween))
	{
		PrimaryComponentTick.SetTickFunctionEnable(true);
	}
	EstimatedTime =  DotBetween / RotationSpeed;

}
