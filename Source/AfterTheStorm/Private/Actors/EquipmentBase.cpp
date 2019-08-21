// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Actors/EquipmentBase.h"

#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

#include "Public/Components/EquipmentComponent.h"
#include "Public/Components/MontageManagerComponent.h"
#include "Public/Components/WeaponTraceComponent.h"

#include "Public/CharacterBase.h"

#include "Public/Utils/ASFunctionLibrary.h"

#include "Engine/World.h"
#include "Public/ASGameInstance.h"

#include "Public/Combat/AttackMove.h"


#include "Public/Utils/ASFunctionLibrary.h"
#include "Classes/SM_State.h"


// Sets default values
AEquipmentBase::AEquipmentBase()
{
}

void AEquipmentBase::BeginPlay()
{
	Super::BeginPlay();
	if (UASGameInstance * Instance = Cast<UASGameInstance>(GetGameInstance()))
	{
		EquipmentData = Instance->GetEquipmentDB()->FindRow<FEquipmentData>(GetID(), "");
		check(EquipmentData != nullptr);
	}
}

void AEquipmentBase::Equip_Implementation(USkeletalMeshComponent* CharacterMesh)
{
	DisableCollisions();
	Mesh->AttachToComponent(CharacterMesh, 
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, 
		UASFunctionLibrary::GetSocketNameFromEquipmentType(GetEquipmentTypeFromDB()));

	Mesh->SetMasterPoseComponent(CharacterMesh);

	EquippedMesh = CharacterMesh;
}

void AEquipmentBase::Unequip_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("EquipmentBase Equip"));
	Destroy();
}

int32 AEquipmentBase::GetAttribute()
{
	return EquipmentData->Attribute;
}

EEquipmentType AEquipmentBase::GetEquipmentTypeFromDB()
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		if (UASGameInstance * GameInstance = Cast<UASGameInstance>(GetGameInstance()))
		{
			UDataTable* EquipmentDB = GameInstance->GetEquipmentDB();
			FEquipmentData* EquipmentData = EquipmentDB->FindRow<FEquipmentData>(GetID(), "");
			if (EquipmentData != nullptr)
			{
				return EquipmentData->Type;
			}
		}
	}
	return EEquipmentType::EET_Invalid;
}

// Sets default values
AWeaponBase::AWeaponBase()
{
	//Create WeaponTraceComponent
	WeaponTraceComponent = CreateDefaultSubobject<UWeaponTraceComponent>("WeaponTraceComponent");
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (UASGameInstance * Instance = Cast<UASGameInstance>(GetGameInstance()))
	{
		WeaponData = Instance->GetWeaponDB()->FindRow<FWeaponData>(GetID(), "");
		check(WeaponData != nullptr);
	}

	CurrentAttackState = InitialAttackState;

	//Setup WeaponTrace Component
	TArray<FName> TraceSockets = Mesh->GetAllSocketNames();
	for (int i = TraceSockets.Num() - 1; i >= 0; --i)
	{
		if (TraceSockets[i].ToString().Contains("Block"))
		{
			TraceSockets.RemoveAt(i);
		}
	}
	WeaponTraceComponent->SetupWeaponTrace(Mesh, TraceSockets);
}

void AWeaponBase::Equip_Implementation(USkeletalMeshComponent* CharacterMesh)
{
	DisableCollisions();

	if (ACharacterBase * Character = Cast<ACharacterBase>(CharacterMesh->GetOwner()))
	{
		SetActorScale3D(Character->GetActorScale3D());
		Mesh->AttachToComponent(CharacterMesh, 
			FAttachmentTransformRules::SnapToTargetNotIncludingScale, 
			Character->GetStanceState() == EStanceState::ESS_Unarmed ? 
				UASFunctionLibrary::GetSheathSocketNameFromEquipmentType(GetEquipmentTypeFromDB()) :
				"hand_r_socket");
		EquippedMesh = CharacterMesh;
	}
}

void AWeaponBase::Unequip_Implementation()
{
	Destroy();
}

void AWeaponBase::PullOut()
{
	WeaponTraceComponent->ActorsToIgnore.AddUnique(this);
	WeaponTraceComponent->ActorsToIgnore.AddUnique(EquippedMesh->GetOwner());

	WeaponTraceComponent->OnWeaponHitEvent.BindUFunction(EquippedMesh->GetOwner(), "OnWeaponHit");

	Mesh->AttachToComponent(EquippedMesh,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		UASFunctionLibrary::GetSocketNameFromEquipmentType(GetEquipmentTypeFromDB()));
}

void AWeaponBase::PutIn()
{
	WeaponTraceComponent->ActorsToIgnore.Empty();
	WeaponTraceComponent->OnWeaponHitEvent.Unbind();

	Mesh->AttachToComponent(EquippedMesh,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		UASFunctionLibrary::GetSheathSocketNameFromEquipmentType(GetEquipmentTypeFromDB()));

}

UAttackMove* AWeaponBase::Attack(EInputBufferKey InputBufferKey)
{
	USM_InputAtom* AttackInput = UASFunctionLibrary::GetAttackInputFromInputKey(InputBufferKey);
	if (AttackInput != nullptr)
	{
		TArray<USM_InputAtom*> InputAtoms;
		InputAtoms.Add(AttackInput);
		FStateMachineResult RunResult = CurrentAttackState->RunState(this, InputAtoms);

		switch (RunResult.CompletionType)
		{
		case EStateMachineCompletionType::Accepted:
		{
			UE_LOG(LogTemp, Warning, TEXT("Accepted"))
			CurrentAttackState = (UAttackMove*)RunResult.FinalState;
			return CurrentAttackState;
		}break;
		case EStateMachineCompletionType::NotAccepted:
		case EStateMachineCompletionType::Rejected:
		{
			UE_LOG(LogTemp, Warning, TEXT("Rejected"))
			CurrentAttackState = InitialAttackState;
			return (UAttackMove*)RunResult.FinalState;
		} break;
		}
	}
	return nullptr;
}

void AWeaponBase::ResetCombo()
{
	CurrentAttackState = InitialAttackState;
}

float AWeaponBase::GetShortRange()
{
	return RangeInfo->ShortRange;
}

float AWeaponBase::GetMediumRange()
{
	return RangeInfo->MediumRange;
}
