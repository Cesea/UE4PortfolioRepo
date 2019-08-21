// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/ExpComponent.h"

// Sets default values for this component's properties
UExpComponent::UExpComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	Level = 1;
	LevelUpExp = 100;
}


// Called when the game starts
void UExpComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UExpComponent::AddExp(int32 Amount)
{
	CurrentExp += Amount;

	if (CurrentExp >= LevelUpExp)
	{
		CurrentExp = CurrentExp - LevelUpExp;
		LevelUpExp = (int32)(LevelUpExp * 1.5f);
		Level += 1;

		OnLevelUp.Broadcast(Level, true);
	}
	OnExpUpdated.Broadcast(CurrentExp, LevelUpExp);
}

