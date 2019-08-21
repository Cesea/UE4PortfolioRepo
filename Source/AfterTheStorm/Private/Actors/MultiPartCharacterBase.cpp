// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Actors/MultiPartCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"

#include "Public/Components/EquipmentComponent.h"


AMultiPartCharacterBase::AMultiPartCharacterBase()
{
	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BodyMesh");
	BodyMesh->SetupAttachment(GetMesh());

	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>("HandMesh");
	HandMesh->SetupAttachment(GetMesh());

	FeetMesh = CreateDefaultSubobject<USkeletalMeshComponent>("FeetMesh");
	FeetMesh->SetupAttachment(GetMesh());

	BodyMesh->SetMasterPoseComponent(GetMesh());
	HandMesh->SetMasterPoseComponent(GetMesh());
	FeetMesh->SetMasterPoseComponent(GetMesh());
}

void AMultiPartCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	EquipmentComponent->OnEquipped.AddDynamic(this, &AMultiPartCharacterBase::OnEquipmentEquipped);
	EquipmentComponent->OnUnequipped.AddDynamic(this, &AMultiPartCharacterBase::OnEquipmentUnequipped);
}

void AMultiPartCharacterBase::OnEquipmentEquipped_Implementation(EEquipmentType EquipmentType, class AEquipmentBase* Equipment)
{
	switch (EquipmentType)
	{
	case EEquipmentType::EET_Invalid:
	{
	} break;
	case EEquipmentType::EET_Weapon:
	{
	} break;
	case EEquipmentType::EET_Shield:
	{
	} break;
	case EEquipmentType::EET_Head:
	{
	} break;
	case EEquipmentType::EET_Body:
	{
		BodyMesh->SetVisibility(false);
	} break;
	case EEquipmentType::EET_Hand:
	{
		HandMesh->SetVisibility(false);
	} break;
	case EEquipmentType::EET_Foot:
	{
		FeetMesh->SetVisibility(false);
	} break;
	case EEquipmentType::EET_Cape:
	{
	} break;
	case EEquipmentType::EET_Props:
	{
	} break;
	default:
	{
	} break;
	}
}

void AMultiPartCharacterBase::OnEquipmentUnequipped_Implementation(EEquipmentType EquipmentType, class AEquipmentBase* Equipment)
{
	switch (EquipmentType)
	{
	case EEquipmentType::EET_Invalid:
		break;
	case EEquipmentType::EET_Weapon:
		break;
	case EEquipmentType::EET_Shield:
		break;
	case EEquipmentType::EET_Head:
		break;
	case EEquipmentType::EET_Body:
		BodyMesh->SetVisibility(true);
		break;
	case EEquipmentType::EET_Hand:
		HandMesh->SetVisibility(true);
		break;
	case EEquipmentType::EET_Foot:
		FeetMesh->SetVisibility(true);
		break;
	case EEquipmentType::EET_Cape:
		break;
	case EEquipmentType::EET_Props:
		break;
	default:
		break;
	}
}