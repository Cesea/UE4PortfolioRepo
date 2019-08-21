// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Actors/SkillActorBase.h"
#include "Public/Interfaces/Attackable.h"

// Sets default values
ASkillActorBase::ASkillActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}


void ASkillActorBase::DamageActor(AActor* ActorToDamage)
{
	if (ActorToDamage != nullptr && !DamagedActors.Contains(ActorToDamage) && ActorToDamage != GetOwner())
	{
		FHitResult HitResult;
		HitResult.Actor = this;
		HitResult.ImpactPoint = ActorToDamage->GetActorLocation();
		HitResult.ImpactNormal = GetActorForwardVector();

		if (IAttackable * Attackable = Cast<IAttackable>(ActorToDamage))
		{
			IAttackable::Execute_HandleHit(ActorToDamage, HitResult, GetOwner(), Damage);
		}
		DamagedActors.AddUnique(ActorToDamage);
	}
}