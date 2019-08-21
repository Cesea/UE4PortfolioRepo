// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Public/Enums/CharacterEnums.h"
#include "Public/Enums/ItemEnums.h"

#include "MontageManagerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERTHESTORM_API UMontageManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMontageManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="MontageManager")
	class UAnimMontage* GetEquipHolsterMontage(EWeaponType WeaponType, EStanceState StanceState);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="MontageManager")
	class UAnimMontage* GetAttackMontage(EWeaponType WeaponType, EAttackType AttackType);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MontageManager")
	class UAnimMontage* GetLightStunMontage() { return LightStunMontage; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="MontageManager")
	class UAnimMontage* GetHeavyStunMontage(EMovementDirection Direction, EStanceState StanceState);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="MontageManager")
	class UAnimMontage* GetDefenseMontage(EWeaponType WeaponType);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="MontageManager")
	class UAnimMontage* GetEvadeMontage() { return EvadeMontage; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="MontageManager")
	class UAnimationAsset* GetDeathAnimation();

protected :
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MontageManager")
	class UMontagesWithWeaponType* EquipMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MontageManager")
	class UMontagesWithWeaponType* HolsterMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MontageManager")
	TMap<EWeaponType, class UMontagesWithAttackType*> AttackMoves;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MontageManager")
	class UMontagesWithWeaponType* DefenseMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MontageManager|Stun")
	class UAnimMontage* LightStunMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MontageManager|Stun")
	class UMontagesWithDirection* HeavyStunMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MontageManager|Evade")
	class UAnimMontage* EvadeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MontageManager|Death")
	class UAnimSequence* DeathAnimation;

};
