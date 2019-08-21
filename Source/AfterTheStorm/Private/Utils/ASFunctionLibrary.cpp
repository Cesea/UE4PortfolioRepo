// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Utils/ASFunctionLibrary.h"

#include "Public/Enums/CharacterEnums.h"
#include "Classes/SM_State.h"

USM_InputAtom* UASFunctionLibrary::AttackInputs[6];

UASFunctionLibrary::UASFunctionLibrary()
{
	int Index = 0;

	if (AttackInputs[Index] == nullptr)
	{
		const FName Path = "SM_InputAtom'/Game/CS_Work/Combats/InputAtoms/IT_LightAttack.IT_LightAttack'";
		AttackInputs[Index] = (USM_InputAtom*)StaticLoadObject(USM_InputAtom::StaticClass(), nullptr, *Path.ToString());
	}

	Index++;
	if (AttackInputs[Index] == nullptr)
	{
		const FName Path = "SM_InputAtom'/Game/CS_Work/Combats/InputAtoms/IT_HeavyAttack.IT_HeavyAttack'";
		AttackInputs[Index] = (USM_InputAtom*)StaticLoadObject(USM_InputAtom::StaticClass(), nullptr, *Path.ToString());
	}

	Index++;
	if (AttackInputs[Index] == nullptr)
	{
		const FName Path = "SM_InputAtom'/Game/CS_Work/Combats/InputAtoms/IT_SpecialAttack01.IT_SpecialAttack01'";
		AttackInputs[Index] = (USM_InputAtom*)StaticLoadObject(USM_InputAtom::StaticClass(), nullptr, *Path.ToString());
	}

	Index++;
	if (AttackInputs[Index] == nullptr)
	{
		const FName Path = "SM_InputAtom'/Game/CS_Work/Combats/InputAtoms/IT_SpecialAttack02.IT_SpecialAttack02'";
		AttackInputs[Index] = (USM_InputAtom*)StaticLoadObject(USM_InputAtom::StaticClass(), nullptr, *Path.ToString());
	}

	Index++;
	if (AttackInputs[Index] == nullptr)
	{
		const FName Path = "SM_InputAtom'/Game/CS_Work/Combats/InputAtoms/IT_SpecialAttack03.IT_SpecialAttack03'";
		AttackInputs[Index] = (USM_InputAtom*)StaticLoadObject(USM_InputAtom::StaticClass(), nullptr, *Path.ToString());
	}

	Index++;
	if (AttackInputs[Index] == nullptr)
	{
		const FName Path = "SM_InputAtom'/Game/CS_Work/Combats/InputAtoms/IT_SpecialAttack04.IT_SpecialAttack04'";
		AttackInputs[Index] = (USM_InputAtom*)StaticLoadObject(USM_InputAtom::StaticClass(), nullptr, *Path.ToString());
	}
}



EInputBufferKey UASFunctionLibrary::AIActionToInputBufferKey(EAIAction Action)
{
	switch (Action)
	{
	case EAIAction::EAA_Invalid:
	{
		return EInputBufferKey::EIBK_Invalid;
	} break;
	case EAIAction::EAA_LightAttack:
	{
		return EInputBufferKey::EIBK_LightAttack;
	} break;
	case EAIAction::EAA_HeavyAttack:
	{
		return EInputBufferKey::EIBK_HeavyAttack;
	} break;
	case EAIAction::EAA_SpecialAttack1:
	{
		return EInputBufferKey::EIBK_SpecialAttack1;
	} break;
	case EAIAction::EAA_SpecialAttack2:
	{
		return EInputBufferKey::EIBK_SpecialAttack2;
	} break;
	case EAIAction::EAA_SpecialAttack3:
	{
		return EInputBufferKey::EIBK_SpecialAttack3;
	} break;
	case EAIAction::EAA_SpecialAttack4:
	{
		return EInputBufferKey::EIBK_SpecialAttack4;
	} break;
	case EAIAction::EAA_ToggleWeapon:
	{
		return EInputBufferKey::EIBK_ToggleStance;
	} break;
	case EAIAction::EAA_Evade:
	{
		return EInputBufferKey::EIBK_JumpOrEvade;
	} break;
	case EAIAction::EAA_Defense:
	{
		return EInputBufferKey::EIBK_Defense;
	} break;
	case EAIAction::EAA_Wait: 
	{
		return EInputBufferKey::EIBK_Invalid;
	}break;
	case EAIAction::EAA_RecoverHealth:
	case EAIAction::EAA_RecoverStamina:
	case EAIAction::EAA_Interact:
	{
		return EInputBufferKey::EIBK_Interact;
	} break;
	default:
	{
		return EInputBufferKey::EIBK_Invalid;
	} break;
	}
}

FName UASFunctionLibrary::GetSocketNameFromEquipmentType(EEquipmentType Type)
{
	switch (Type)
	{
	case EEquipmentType::EET_Weapon:
		return "hand_r_socket";
		break;
	case EEquipmentType::EET_Shield:
		return "hand_l_socket";
		break;
	case EEquipmentType::EET_Head:
		return "head_socket";
		break;
	case EEquipmentType::EET_Body:
		return "body_socket";
		break;
	case EEquipmentType::EET_Hand:
		return "forearm_socket";
		break;
	case EEquipmentType::EET_Foot:
		return "foot_socket";
		break;
	default:
		return "";
		break;
	}
}

//Might add Greatsword
FName UASFunctionLibrary::GetSheathSocketNameFromEquipmentType(EEquipmentType Type)
{
	if (Type == EEquipmentType::EET_Weapon)
	{
		return "sword_sheath_socket";
	}
	else if (Type == EEquipmentType::EET_Shield)
	{
		return "shield_sheath_socket";
	}
	else
	{
		return "";
	}
}

USM_InputAtom* UASFunctionLibrary::GetAttackInputFromInputKey(EInputBufferKey InputBufferKey)
{
	return AttackInputs[(uint8)(InputBufferKey) - 1];
}
