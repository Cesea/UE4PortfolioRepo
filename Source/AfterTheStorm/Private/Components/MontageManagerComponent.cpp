// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/MontageManagerComponent.h"

#include "Animation/AnimMontage.h"

#include "Public/Combat/AttackMove.h"

// Sets default values for this component's properties
UMontageManagerComponent::UMontageManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	// ...
}


// Called when the game starts
void UMontageManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	//check(EquipMontages != nullptr);
	//check(HolsterMontages != nullptr);
	// ...
}

UAnimMontage* UMontageManagerComponent::GetEquipHolsterMontage(EWeaponType WeaponType, EStanceState StanceState)
{
	return (StanceState == EStanceState::ESS_Armed) ? 
		EquipMontages->FindMontage(WeaponType) : HolsterMontages->FindMontage(WeaponType);
}

UAnimMontage* UMontageManagerComponent::GetAttackMontage(EWeaponType WeaponType, EAttackType AttackType)
{
	return AttackMoves[WeaponType]->FindMontage(AttackType);
}

UAnimMontage* UMontageManagerComponent::GetHeavyStunMontage(EMovementDirection Direction, EStanceState StanceState)
{
	return HeavyStunMontages->GetMontageByDirection(Direction);
}

UAnimMontage* UMontageManagerComponent::GetDefenseMontage(EWeaponType WeaponType)
{
	return DefenseMontages->FindMontage(WeaponType);
}

UAnimationAsset* UMontageManagerComponent::GetDeathAnimation()
{
	return (UAnimationAsset* )DeathAnimation;
}

