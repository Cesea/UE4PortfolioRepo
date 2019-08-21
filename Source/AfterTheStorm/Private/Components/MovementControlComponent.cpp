// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/MovementControlComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UMovementControlComponent::UMovementControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
 // ...
}


// Called when the game starts
void UMovementControlComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterMovementComponentRef = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();

	MovementState = EMovementState::EMS_Run;
	SetMovementState(EMovementState::EMS_Walk);
}

void UMovementControlComponent::SetMovementState(EMovementState NewState)
{
	if (CharacterMovementComponentRef != nullptr)
	{
		switch (NewState)
		{
		case EMovementState::EMS_Walk:
		{
			CharacterMovementComponentRef->MaxWalkSpeed = MovementSpeed.WalkSpeed;
		}break;
		case EMovementState::EMS_Run:
		{
			CharacterMovementComponentRef->MaxWalkSpeed = MovementSpeed.RunSpeed;
		} break;
		case EMovementState::EMS_Sprint:
		{
			CharacterMovementComponentRef->MaxWalkSpeed = MovementSpeed.SprintSpeed;
		} break;
		default:
			break;
		}
	}
	MovementState = NewState;
}

