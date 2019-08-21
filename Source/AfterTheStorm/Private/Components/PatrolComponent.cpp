// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/PatrolComponent.h"

#include "Public/Actors/PatrolPoint.h"

// Sets default values for this component's properties
UPatrolComponent::UPatrolComponent()
{
	bMovingRandomly = false;
}


// Called when the game starts
void UPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

FVector UPatrolComponent::ToNextPatrolPoint()
{
	if(PatrolPoints.Num() == 0)
	{
		return FVector::ZeroVector;
	}

	int32 NextIndex = CurrentPatrolIndex + 1;
	if (NextIndex >= PatrolPoints.Num())
	{
		NextIndex = 0;
	}
	CurrentPatrolIndex = NextIndex;

	return PatrolPoints[CurrentPatrolIndex]->GetActorLocation();
}


// Called every frame
//void UPatrolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//	// ...
//}

