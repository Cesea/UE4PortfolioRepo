// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/WeaponTraceComponent.h"

#include "Components/SkeletalMeshComponent.h"

#include "Public/Interfaces/Attackable.h"
#include "Kismet/KismetMathLibrary.h"

#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
UWeaponTraceComponent::UWeaponTraceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UWeaponTraceComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UWeaponTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bShouldTrace)
	{
		PerformTrace();
	}
	if (bShouldUpdateSocketLocations)
	{
		UpdateLastSocketLocations();
	}
}

void UWeaponTraceComponent::SetupWeaponTrace(USkeletalMeshComponent* WeaponMesh, TArray<FName> Sockets)
{
	Mesh = WeaponMesh;
	SocketNames = Sockets;
}

void UWeaponTraceComponent::ActivateCollision()
{
	bShouldTrace = true;
	bShouldUpdateSocketLocations = true;
	UpdateLastSocketLocations();
	ActorsHit.Empty();
}

void UWeaponTraceComponent::DeactivateCollision()
{
	bShouldTrace = false;
	bShouldUpdateSocketLocations = false;
	ActorsHit.Empty();
}

void UWeaponTraceComponent::UpdateLastSocketLocations()
{
	for (const FName& Socket : SocketNames)
	{
		LastSocketLocations.Add(Socket, Mesh->GetSocketLocation(Socket));
	}
}

void UWeaponTraceComponent::PerformTrace()
{
	if (bShouldTrace)
	{
		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			TArray<FHitResult> HitResults;

			FCollisionObjectQueryParams QueryParams;
			QueryParams.AddObjectTypesToQuery(ECC_Pawn);

			for (const FName& Socket : SocketNames)
			{
				if (bDrawDebugLines)
				{
					DrawDebugLine(World, *LastSocketLocations.Find(Socket), Mesh->GetSocketLocation(Socket), FColor::Red, false, 0.5f, 0, 0.5f);
				}

				if (World->LineTraceMultiByObjectType(HitResults,
					*LastSocketLocations.Find(Socket), Mesh->GetSocketLocation(Socket),
					QueryParams, FCollisionQueryParams::DefaultQueryParam))
				{
					for (const FHitResult& Hit : HitResults)
					{
						if (IsIgnoredClass(Hit.GetActor()->GetClass()) || IsIgnoredActor(Hit.GetActor()))
						{
							continue;
						}

						if (ActorsHit.Contains(Hit.GetActor()))
						{
							continue;
						}

						IAttackable* Attackable = Cast<IAttackable>(Hit.GetActor());

						if (Attackable != nullptr &&
							!ActorsHit.Contains(Hit.GetActor()))
						{
							ActorsHit.Add(Hit.GetActor());
							OnWeaponHitEvent.ExecuteIfBound(Hit);
						}
					}
				}
				HitResults.Empty();
			}
		}
	}
}

bool UWeaponTraceComponent::IsIgnoredClass(TSubclassOf<AActor> TestClass)
{
	if (ClassesToIgnore.Contains(TestClass))
		return true;

	for(TSubclassOf<AActor> Class : ClassesToIgnore)
	{
		if (UKismetMathLibrary::ClassIsChildOf(TestClass, Class))
		{
			return true;
		}
	}

	return false;
}

bool UWeaponTraceComponent::IsIgnoredActor(AActor* Actor)
{
	for(AActor* ActorToIgnore : ActorsToIgnore)
	{
		if (ActorToIgnore == Actor)
		{
			return true;
		}
	}

	return false;
}

