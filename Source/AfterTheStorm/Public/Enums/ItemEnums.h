// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemEnums.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Invalid		UMETA(Hidden),
	EIT_Weapon		UMETA(DisplayName="Weapon"),
	EIT_Armor		UMETA(DisplayName="Armor"),
	EIT_Usable		UMETA(DisplayName="Usable"),
	EIT_Quest		UMETA(DisplayName="Quest"),
	EIT_Misc		UMETA(DisplayName="Misc"),
};


UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	EET_Invalid		UMETA(Hidden),
	EET_Weapon		UMETA(DisplayName="Weapon"),
	EET_Shield		UMETA(DisplayName="Shield"),
	EET_Head		UMETA(DisplayName="Head"),
	EET_Body		UMETA(DisplayName ="Body"),
	EET_Hand		UMETA(DisplayName="Hand"),
	EET_Foot		UMETA(DisplayName="Foot"),
	EET_Cape		UMETA(DisplayName ="Cape"),
	EET_Props		UMETA(DisplayName ="Props"),
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Invalid		UMETA(Hidden),
	EWT_Dagger		UMETA(DisplayName="Dagger"),
	EWT_Sword		UMETA(DisplayName="Sword"),
	EWT_GreatSword	UMETA(DisplayName="GreatSword"),
	EWT_Body		UMETA(Displayname="Body")
};


UENUM(BlueprintType)
enum class EEffectTarget : uint8
{
	EET_Health		UMETA(DisplayName="Health"),
	EET_Stamina		UMETA(DisplayName="Stamina"),
	EET_Strength	UMETA(DisplayName="Strength"),
	EET_Defense		UMETA(DisplayName="Defense"),
};

UENUM(BlueprintType)
enum class EEffectType : uint8
{
	EET_Invalid		UMETA(Hidden),
	EET_Add			UMETA(DisplayName="Add"),
	EET_Sub			UMETA(DisplayName="Sub"),
	EET_Mult		UMETA(DisplayName="Mult"),
};


UENUM(BlueprintType)
enum class EEquipResult : uint8
{
	EER_Invalid							UMETA(Hidden),
	EER_Success							UMETA(DisplayName="Success"),
	EER_UnEquippedOtherAndEquipped		UMETA(DisplayName="UnEquippedOtherAndEquipped"),
	EER_UnEquipped						UMETA(DisplayName="UnEquipped"),
	EER_FailedToEqiup					UMETA(DisplayName="FailedToEquip")
};
