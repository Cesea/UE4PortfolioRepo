// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Actors/UsableBase.h"

#include "Public/CharacterBase.h"

#include "Public/Components/StatComponent.h"

// Sets default values
AUsableBase::AUsableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

}

// Called when the game starts or when spawned
void AUsableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUsableBase::Use_Implementation(AActor* Actor)
{
	if (ACharacterBase * CharacterBase = Cast<ACharacterBase>(Actor))
	{
		switch (EffectTarget)
		{
		case EEffectTarget::EET_Health:
		{
			CharacterBase->HealthStatComponent->AddStat(EffectAmount);
		} break;
		case EEffectTarget::EET_Stamina:
		{
			CharacterBase->StaminaStatComponent->AddStat(EffectAmount);
		} break;
		case EEffectTarget::EET_Strength:
		{
		} break;
		case EEffectTarget::EET_Defense:
		{
		} break;
		default:
			break;
		}
	}
}

