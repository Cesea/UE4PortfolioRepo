// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Actors/NPCs/BossBase.h"

#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

#include "Public/Components/EquipmentComponent.h"
#include "Public/Components/MontageManagerComponent.h"


bool ABossBase::CanBeStuned()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr &&
		AnimInstance->Montage_IsPlaying(nullptr))
	{
		UAnimMontage* SpecialOneMontage = MontageManagerComponent->GetAttackMontage(EWeaponType::EWT_GreatSword, EAttackType::EAT_SpecialAttack1);
		UAnimMontage* SpecialTwoMontage = MontageManagerComponent->GetAttackMontage(EWeaponType::EWT_GreatSword, EAttackType::EAT_SpecialAttack2);
		if (AnimInstance->Montage_IsPlaying(SpecialOneMontage) ||
			AnimInstance->Montage_IsPlaying(SpecialTwoMontage))
		{
			return false;
		}
	}

	return Super::CanBeStuned();
}
