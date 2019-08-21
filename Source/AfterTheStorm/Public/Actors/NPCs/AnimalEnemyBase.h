// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/NPCs/EnemyBase.h"
#include "AnimalEnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API AAnimalEnemyBase : public AEnemyBase
{
	GENERATED_BODY()

protected :

	AAnimalEnemyBase();
	virtual void BeginPlay() override;

public :

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWeaponTraceComponent* WeaponTraceComponent;

	virtual void OnWeaponHit_Implementation(const FHitResult& HitResult) override;
	virtual void MainAttack_Implementation(EInputBufferKey InputKey) override;
	virtual void ToggleWeapon_Implementation() override;

protected :
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TArray<FName> AttackTraceSockets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float Damage{ 10 };
	
};
