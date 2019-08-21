// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/InteractionComponent.h"

#include "GameFramework/Actor.h"
#include "Public/Interfaces/Interactable.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	FocusedIndex = -1;

	bAutoActivate = true;
	// ...
	
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (InteractableActors.Num() > 0)
	{
		UpdateFocusedIndex(FindFocusedActorIndex());
	}
}

void UInteractionComponent::AddInteractable(AActor* Actor)
{
	if (IInteractable * Interactable = Cast<IInteractable>(Actor))
	{
		InteractableActors.AddUnique(Actor);
		UpdateFocusedIndex(FindFocusedActorIndex());
	}
}

void UInteractionComponent::RemoveInteractable(AActor* Actor)
{
	if (IInteractable * Interactable = Cast<IInteractable>(Actor))
	{
		//execute interface first
		IInteractable::Execute_HideFeedbackWidget(Actor, GetOwner());
		//Remove actor from array
		InteractableActors.Remove(Actor);
		//Reset index
		FocusedIndex = -1;

		UpdateFocusedIndex(FindFocusedActorIndex());
	}
}

void UInteractionComponent::Interact()
{
	if (FocusedIndex >= 0)
	{
		InteractableActors[FocusedIndex];
		IInteractable::Execute_Interact(InteractableActors[FocusedIndex], GetOwner());
	}
}

int UInteractionComponent::FindFocusedActorIndex()
{
	if (InteractableActors.Num() == 0)
	{
		return -1;
	}
	else if (InteractableActors.Num() == 1)
	{
		return 0;
	}
	else
	{
		int BestIndex = 0;
		float BestDotProductValue = -1.0f;

		FVector OwnerRotation = GetOwner()->GetActorRotation().Vector();

		for (int i = 0; i < InteractableActors.Num(); ++i)
		{
			FVector PlayerToInteractable = (InteractableActors[i]->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
			float DotValue = FVector::DotProduct(OwnerRotation, PlayerToInteractable);

			if (DotValue > BestDotProductValue)
			{
				BestDotProductValue = DotValue;
				BestIndex = i;
			}
		}
		return BestIndex;
	}
}

void UInteractionComponent::UpdateFocusedIndex(int NewIndex)
{
	if (FocusedIndex != NewIndex)
	{
		if (FocusedIndex >= 0)
		{
			IInteractable::Execute_HideFeedbackWidget(InteractableActors[FocusedIndex], GetOwner());
		}
		
		if (NewIndex >= 0)
		{
			IInteractable::Execute_ShowFeedbackWidget(InteractableActors[NewIndex], GetOwner());
		}
		FocusedIndex = NewIndex;
	}
}

