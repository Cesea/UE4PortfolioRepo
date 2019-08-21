// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/StatComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

#include "Public/MyGameState.h"
#include "Public/Components/GlobalEventHandlerComponent.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	// ...
	PrimaryComponentTick.bCanEverTick = true;

	Current = 100.0f;
	Max = 100.0f;

	RegenDelaySeconds = 0;
	RegenPerSeconds = 0;
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	PrimaryComponentTick.SetTickFunctionEnable(RegenPerSeconds == 0.0f ? false : true);

	UpdateStatState();


	OwnedByPlayer = GetOwner()->ActorHasTag("Player");

	UWorld* World = GetWorld();
	if (World)
	{
		if (AMyGameState * GameState = Cast<AMyGameState>(World->GetGameState()))
		{
			GlobalEventHandlerComponentRef = GameState->GlobalEventHandlerComponent;
			check(GlobalEventHandlerComponentRef != nullptr);
		}
	}
}

void UStatComponent::InitializeValues(float NewCurrent, float NewMax)
{
	Current = NewCurrent;
	Max = NewMax;
}

void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (DelayAccum > RegenDelaySeconds)
	{
		AddStat(RegenPerSeconds * DeltaTime);
		if (Current == Max)
		{
			PrimaryComponentTick.SetTickFunctionEnable(false);
		}
	}
	DelayAccum += DeltaTime;
}

void UStatComponent::AddStat(float Amount)
{
	if (Amount < 0.0f && RegenPerSeconds > 0.0f)
	{	
		PrimaryComponentTick.SetTickFunctionEnable(true);
		DelayAccum = 0.0f;
	}

	float TempCurrent = Current + Amount;
	TempCurrent = FMath::Clamp<float>(TempCurrent, 0, Max);
	SetCurrentValue(TempCurrent);
}

void UStatComponent::SubStat(float Amount)
{
	AddStat(-Amount);
}

void UStatComponent::SetCurrentValue(float NewValue)
{
	Current = NewValue;

	OnStatChanged.Broadcast(Current, Max);

	UpdateStatState();

	if (Current <= 0.0f)
	{
		OnStatZeroEvent.Broadcast();
	}
}

void UStatComponent::SetMaxValue(float NewValue)
{
	Max = NewValue;
	OnStatChanged.Broadcast(Current, Max);
}

void UStatComponent::Regenerate()
{
}

void UStatComponent::UpdateStatState()
{
	float Percent = Current / Max;
	EStatState NewStatState = Percent > 0.7f ? EStatState::ESS_Full : Percent > 0.4f ? EStatState::ESS_Medium : EStatState::ESS_Short;
	if (PrevStatState != NewStatState)
	{
		OnStatStateChanged.Broadcast(NewStatState, Percent);
		PrevStatState = NewStatState;
	}
}
