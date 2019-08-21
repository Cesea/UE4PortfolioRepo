// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Animation/AnimMontage.h"
#include "Classes/SM_State.h"

#include "Public/Enums/CharacterEnums.h"
#include "Public/Enums/ItemEnums.h"

#include "AttackMove.generated.h"

/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API UMontagesWithWeaponType : public UDataAsset
{
	GENERATED_BODY()

public :
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UAnimMontage* FindMontage(EWeaponType Type) { return Montages[Type]; }

protected :
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EWeaponType, UAnimMontage*> Montages;
};

UCLASS()
class AFTERTHESTORM_API UMontagesWithAttackType : public UDataAsset
{
	GENERATED_BODY()

public :
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UAnimMontage* FindMontage(EAttackType Type) { return Montages[Type]; }

protected :
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EAttackType, UAnimMontage*> Montages;
};


UCLASS()
class AFTERTHESTORM_API UAttackMove : public USM_State
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAttackType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageMultiply{1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StaminaCost;
};

UCLASS()
class AFTERTHESTORM_API UMontagesWithDirection : public UDataAsset
{
	GENERATED_BODY()

public :
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UAnimMontage* GetMontageByDirection(EMovementDirection Direction) { return Montages[(uint8)Direction - 1]; }

protected :
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UAnimMontage*> Montages;
};


UCLASS()
class AFTERTHESTORM_API UAttackRangeInfo : public UDataAsset
{
	GENERATED_BODY()

public :
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float ShortRange{250};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MediumRange{450};
};
