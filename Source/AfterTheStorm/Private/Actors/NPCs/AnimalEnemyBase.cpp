// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Actors/NPCs/AnimalEnemyBase.h"

#include "Components/SkeletalMeshComponent.h"

#include "Animation/AnimInstance.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"

#include "Public/Actors/NPCs/EnemyControllerBase.h"

#include "Public/Components/WeaponTraceComponent.h"
#include "Public/Components/MontageManagerComponent.h"
#include "Public/Components/StateMachineComponent.h"


AAnimalEnemyBase::AAnimalEnemyBase()
{
	WeaponTraceComponent = CreateDefaultSubobject<UWeaponTraceComponent>("WeaponTraceComponent");
}

void AAnimalEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	check(AttackTraceSockets.Num() > 0);
	WeaponTraceComponent->SetupWeaponTrace(GetMesh(), AttackTraceSockets);

	UBlackboardComponent* Blackboard = ControllerRef->GetBlackboardComponent();
	if (Blackboard != nullptr)
	{
		Blackboard->SetValueAsFloat("WeaponShortRange", 200.0f);
		Blackboard->SetValueAsFloat("WeaponMediumRange", 550.0f);
	}

}

void AAnimalEnemyBase::OnWeaponHit_Implementation(const FHitResult& HitResult)
{
	if (IAttackable* Attackable = Cast<IAttackable>(HitResult.GetActor()))
	{
		if (Attackable->IsVulnerable())
		{
			IAttackable::Execute_HandleHit(HitResult.GetActor(), HitResult, this, Damage);
			OnHitSuccess(HitResult);
		}
	}
}

void AAnimalEnemyBase::MainAttack_Implementation(EInputBufferKey InputKey)
{
	if(CanAttack())
	{
		UAnimMontage* MontageToPlay = nullptr;
		if (InputKey == EInputBufferKey::EIBK_LightAttack)
		{
			MontageToPlay = MontageManagerComponent->GetAttackMontage(EWeaponType::EWT_Body, EAttackType::EAT_LightAttack1);
		}
		else if (InputKey == EInputBufferKey::EIBK_HeavyAttack)
		{
			MontageToPlay = MontageManagerComponent->GetAttackMontage(EWeaponType::EWT_Body, EAttackType::EAT_HeavyAttack1);
		}
		if (MontageToPlay != nullptr)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance != nullptr &&
				MontageToPlay != nullptr)
			{
				AnimInstance->Montage_Play(MontageToPlay);

				AttackType = (EAttackType)InputKey;
				StateMachineComponent->SetState(EState::ES_Attack);
			}
		}
	}
}

void AAnimalEnemyBase::ToggleWeapon_Implementation()
{
	if (CanToggleWeapon())
	{
		if (StanceState == EStanceState::ESS_Unarmed)
		{
			StanceState = EStanceState::ESS_Armed;
			WeaponTraceComponent->ActorsToIgnore.Add(this);
			WeaponTraceComponent->OnWeaponHitEvent.BindUFunction(this, "OnWeaponHit");
		}
		else if (StanceState == EStanceState::ESS_Armed)
		{
			StanceState = EStanceState::ESS_Unarmed;
			WeaponTraceComponent->ActorsToIgnore.Empty();
			WeaponTraceComponent->OnWeaponHitEvent.Clear();
		}
	}
}

