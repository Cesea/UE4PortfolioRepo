// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/InputBufferComponent.h"

// Sets default values for this component's properties
UInputBufferComponent::UInputBufferComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	// ...
}


// Called when the game starts
void UInputBufferComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UInputBufferComponent::OpenInputBuffer()
{
	if (!bIsOpen)
	{
		bIsOpen = true;
		OnOpenEvent.Broadcast();
	}
}

void UInputBufferComponent::CloseInputBuffer()
{
	if (bIsOpen)
	{
		bIsOpen = false;
		OnCloseEvent.Broadcast();
		ConsumeInputBuffer();
	}
}

void UInputBufferComponent::ConsumeInputBuffer()
{
	OnConsumedEvent.Broadcast(StoredKey);
	StoredKey = EInputBufferKey::EIBK_Invalid;
}

void UInputBufferComponent::UpdateKey(EInputBufferKey NewKey)
{
	StoredKey = NewKey;
	if (!bIsOpen)
	{
		ConsumeInputBuffer();
	}
}

