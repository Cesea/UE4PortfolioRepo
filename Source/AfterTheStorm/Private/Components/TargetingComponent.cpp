// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/TargetingComponent.h"

#include "Public/Interfaces/Targetable.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"


#include "Public/Actors/Players/PlayerCharacter.h"

// Sets default values for this component's properties
UTargetingComponent::UTargetingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.SetTickFunctionEnable(false);

	TargetSearchDistance = 800.0f;
	TargetSearchDegree = 80.0f;
	TargetLooseDistance = 800.0f;
}


// Called when the game starts
void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	ControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (APawn * CastedOwner = Cast<APawn>(GetOwner()))
	{
		PlayerCharacterRef = Cast<APlayerCharacter>(CastedOwner);
	}

	check(PlayerCharacterRef != nullptr);
	check(ControllerRef != nullptr);

	ActorsToIgnore.Add(GetOwner());
}

void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (IsTargeted())
	{
		APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FVector2D Delta;

		Controller->GetInputMouseDelta(Delta.X, Delta.Y);

		float DeltaSize = Delta.Size();

		if (DeltaSize > 5.0f && PrevInputDeltaLength < 5.0f)
		{
			ChangeTarget(Delta);
		}
		PrevInputDeltaLength = DeltaSize;
	}
}

void UTargetingComponent::LockTarget()
{
	bool Result = false;
	AActor* ClosestActor = FindTargetActor();

	if (ClosestActor != nullptr)
	{
		ITargetable::Execute_OnTargeted(ClosestActor, GetOwner());

		ControllerRef->SetIgnoreLookInput(true);

		TargetedActor = ClosestActor;
		Result = true;

		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(CheckTargetTimerHandle, this, &UTargetingComponent::CheckTargetActor, 0.15f, true);

		PrimaryComponentTick.SetTickFunctionEnable(true);
	}

	PlayerCharacterRef->OnTargetingBegin(Result);
}

void UTargetingComponent::ChangeTarget(FVector2D ScreenDirection)
{
	AActor* NewTarget = FindActorOnScreenDirection(ScreenDirection);
	if (NewTarget != nullptr)
	{
		ITargetable::Execute_OnReleasedTarget(TargetedActor, GetOwner());
		TargetedActor = NewTarget;
		ITargetable::Execute_OnTargeted(TargetedActor, GetOwner());
	}
}

void UTargetingComponent::UnlockTarget()
{
	if (TargetedActor != nullptr)
	{
		ControllerRef->ResetIgnoreLookInput();
		ITargetable::Execute_OnReleasedTarget(TargetedActor, GetOwner());

		CheckTargetTimerHandle.Invalidate();

		TargetedActor = nullptr;

		PrimaryComponentTick.SetTickFunctionEnable(false);
		PlayerCharacterRef->OnTargetingEnd();
	}
}

void UTargetingComponent::CheckTargetActor()
{
	if (TargetedActor != nullptr)
	{
		bool IsVisible = CheckVisibility(TargetedActor);
		bool IsInRange = CheckDistance(TargetedActor);

		if (!IsVisible || !IsInRange)
		{
			UnlockTarget();
		}
	}
}

AActor* UTargetingComponent::FindTargetActor()
{
	TArray<AActor*> OverlappingActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetOwner()->GetActorLocation(),
		TargetSearchDistance, ObjectTypesToQuery, nullptr, ActorsToIgnore, OverlappingActors);

	AActor* ClosestActor = nullptr;
	float ClosestDistance = 9990.0f;

	FVector OwnerLocation = GetOwner()->GetActorLocation();
	FVector ForwardVector = GetOwner()->GetActorForwardVector();
	FVector TestLocation;

	for (auto& Actor : OverlappingActors)
	{
		TestLocation = Actor->GetActorLocation();
		float TestDistance = FVector::Distance(TestLocation, OwnerLocation);
		ITargetable* Targetable = Cast<ITargetable>(Actor);
		if (Targetable != nullptr)
		{
			FVector2D ScreenPosition;
			bool IsInScreen =
				UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(), 0), Actor->GetActorLocation(), ScreenPosition);
			bool IsSeeable = CheckVisibility(Actor);

			if ((TestDistance < ClosestDistance) && (IsInScreen) && IsSeeable)
			{
				ClosestDistance = TestDistance;
				ClosestActor = Actor;
			}
		}
	}
	return ClosestActor;
}

AActor* UTargetingComponent::FindActorOnScreenDirection(FVector2D ScreenDirection)
{
	float BestDot = 0.0f;
	AActor* BestActor = nullptr;

	if (TargetedActor != nullptr)
	{
		FVector2D CurrentTargetScreenPos;
		UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(), 0),
												TargetedActor->GetActorLocation(), CurrentTargetScreenPos);

		TArray<AActor*> OverlappingActors;
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetOwner()->GetActorLocation(),
			TargetSearchDistance, ObjectTypesToQuery, nullptr, ActorsToIgnore, OverlappingActors);

		FVector2D LocalScreenPos;
		
		for (AActor* Actor : OverlappingActors)
		{
			if (UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(), 0),
				Actor->GetActorLocation(), LocalScreenPos))
			{
				FVector2D Direction2D = (LocalScreenPos - CurrentTargetScreenPos).GetSafeNormal();

				float LocalDot = FVector2D::DotProduct(Direction2D, ScreenDirection);
				if (LocalDot > BestDot)
				{
					BestDot = LocalDot;
					BestActor = Actor;
				}
			}
		}
	}
	return BestActor;
}

bool UTargetingComponent::CheckVisibility(AActor* Actor)
{
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (CameraManager != nullptr)
	{
		UWorld* World = GetWorld();
		FHitResult HitResult;
		return World->LineTraceSingleByChannel(HitResult, CameraManager->GetCameraLocation(), Actor->GetActorLocation(), ECollisionChannel::ECC_Pawn);
	}
	return false;
}

bool UTargetingComponent::CheckDistance(AActor* Actor)
{
	return FVector::Distance(Actor->GetActorLocation(), GetOwner()->GetActorLocation()) < TargetLooseDistance;
}
