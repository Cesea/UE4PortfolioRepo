// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterEnums.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAT_Invalid				UMETA(Hidden, DisplayName="Invalid"),
	EAT_LightAttack1		UMETA(DisplayName="LightAttack1"),
	EAT_LightAttack2		UMETA(DisplayName="LightAttack2"),
	EAT_LightAttack3		UMETA(DisplayName="LightAttack3"),
	EAT_LightAttack4		UMETA(DisplayName="LightAttack4"),
	EAT_HeavyAttack1		UMETA(DisplayName="HeavyAttack1"),
	EAT_HeavyAttack2		UMETA(DisplayName="HeavyAttack2"),
	EAT_HeavyAttack3		UMETA(DisplayName="HeavyAttack3"),
	EAT_HeavyAttack4		UMETA(DisplayName="HeavyAttack4"),
	EAT_SpecialAttack1		UMETA(DisplayName="SpecialAttack1"),
	EAT_SpecialAttack2		UMETA(DisplayName="SpecialAttack2"),
	EAT_SpecialAttack3		UMETA(DisplayName="SpecialAttack3"),
	EAT_SpecialAttack4		UMETA(DisplayName="SpecialAttack4"),
};

UENUM(BlueprintType)
enum class EMovementDirection : uint8
{
	EMD_Invalid			UMETA(Hidden),
	EMD_Forward			UMETA(DisplayName="Forward"),
	EMD_Backward		UMETA(DisplayName="Bacward"),
	EMD_Left			UMETA(DisplayName="Left"),
	EMD_Right			UMETA(DisplayName="Right"),
};

UENUM(BlueprintType)
enum class EInputBufferKey :uint8
{
	EIBK_Invalid				UMETA(DisplayName="Invalid"),
	EIBK_LightAttack			UMETA(DisplayName="LightAttack"),
	EIBK_HeavyAttack			UMETA(DisplayName="HeavyAttack"),
	EIBK_SpecialAttack1			UMETA(DisplayName="SpecialAttack1"),
	EIBK_SpecialAttack2			UMETA(DisplayName="SpecialAttack2"),
	EIBK_SpecialAttack3			UMETA(DisplayName="SpecialAttack3"),
	EIBK_SpecialAttack4			UMETA(DisplayName="SpecialAttack4"),
	EIBK_Defense				UMETA(DisplayName="Defense"),
	EIBK_ToggleStance			UMETA(DisplayName="ToggleStance"),
	EIBK_JumpOrEvade			UMETA(DisplayName="JumpOrEvade"),
	EIBK_Interact				UMETA(DisplayName="Interact"),
	EIBK_Crouch					UMETA(DisplayName="Crouch"),
	EIBK_ToggleWalkRun			UMETA(DisplayName="ToggleWalkRun"),
	EIBK_StartSprint			UMETA(DisplayName="StartSprint"),
	EIBK_StopSprint				UMETA(DisplayName="StopSprint"),
};


UENUM(BlueprintType)
enum class EState : uint8
{
	ES_Stand,
	ES_Crouch,
	ES_Stun,
	ES_Attack,
	ES_Evade,
	ES_Jump,
	ES_Interact,
	ES_Dead,
};

UENUM(BlueprintType)
enum class EStanceState : uint8
{
	ESS_Unarmed		UMETA(DisplayName="Unarmed"),
	ESS_Armed		UMETA(DisplayName="Armed"),
};

UENUM(BlueprintType)
enum class EAIState : uint8
{
	EAS_Invalid		UMETA(Hidden),
	EAS_Idling		UMETA(DisplayName = "Idling"),
	EAS_Patrolling	UMETA(DisplayName = "Patrolling"),
	EAS_Chasing		UMETA(DisplayName = "Chasing"),
	EAS_Combating	UMETA(DisplayName = "Combating"),
	EAS_Searcging	UMETA(DisplayName = "Searching"),
	EAS_Fleeing		UMETA(DisplayName = "Fleeing"),
	EAS_Stuning		UMETA(DisplayName = "Stuning"),
	EAS_Defensing	UMETA(DisplayName = "Defensing"),
	EAS_Interacting	UMETA(DisplayName = "Interacting"),
	EAS_Dead		UMETA(Displayname = "Dead"),
};

UENUM(BlueprintType, meta = (Bitflags))
enum class EAIAction : uint8
{
	EAA_Invalid				UMETA(DisplayName="Invalid"),
	EAA_LightAttack			UMETA(DisplayName="LightAttack"),
	EAA_HeavyAttack			UMETA(DisplayName="HeavyAttack"),
	EAA_SpecialAttack1		UMETA(DisplayName="SpecialAttack1"),
	EAA_SpecialAttack2		UMETA(DisplayName="SpecialAttack2"),
	EAA_SpecialAttack3		UMETA(DisplayName="SpecialAttack3"),
	EAA_SpecialAttack4		UMETA(DisplayName="SpecialAttack4"),
	EAA_ToggleWeapon		UMETA(DisplayName="ToggleWeapon"),
	EAA_Evade				UMETA(DisplayName="Evade"),
	EAA_Defense				UMETA(DisplayName="Defense"),
	EAA_Wait				UMETA(DisplayName="Wait"),
	EAA_RecoverHealth		UMETA(DisplayName="RecoverHealth"),
	EAA_RecoverStamina		UMETA(DisplayName="RecoverStamina"),
	EAA_Interact			UMETA(DisplayName="Interact"),
};

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	EMS_Walk,
	EMS_Run,
	EMS_Sprint,
};

UENUM(BlueprintType)
enum class EDistanceToTarget : uint8
{
	EDT_Invalid		UMETA(Hidden),
	EDT_Short		UMETA(DisplayName = "Short"),
	EDT_Medium		UMETA(DisplayName = "Medium"),
	EDT_Far			UMETA(DisplayName = "Far"),
	EDT_Over		UMETA(DisplayName = "Over"),
};

UENUM(BlueprintType)
enum class EStatState : uint8
{
	ESS_Invalid		UMETA(Hidden),
	ESS_Short		UMETA(DisplayName="Short"),
	ESS_Medium		UMETA(DisplayName="Medium"),
	ESS_Full		UMETA(DisplayName="Full"),
};
