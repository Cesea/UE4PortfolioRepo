// Fill out your copyright notice in the Description page of Project Settings.

#include "EquipmentComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Public/Actors/EquipmentBase.h"

#include "Public/Components/InventoryComponent.h"

#include "Engine/World.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	//Find Components in owner
	SkeletalMeshComponentRef = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
}

void UEquipmentComponent::Equip(EEquipmentType Type, TSubclassOf<class AEquipmentBase> EquipmentClass, int32 IndexInInventory)
{
	SetEquipmentClass(Type, EquipmentClass);
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		AEquipmentBase* SpawnedEquipment = World->SpawnActor<AEquipmentBase>(EquipmentClass);
		SpawnedEquipment->SetIndexInInventory(IndexInInventory);

		SetEquipmentActor(Type, SpawnedEquipment);
		if (IEquipable * Equipable = Cast<IEquipable>(SpawnedEquipment))
		{
			OnEquipped.Broadcast(Type, SpawnedEquipment);
			IEquipable::Execute_Equip(SpawnedEquipment, GetOwner()->FindComponentByClass<USkeletalMeshComponent>());
		}
	}
}

void UEquipmentComponent::Unequip(EEquipmentType Type)
{
	AEquipmentBase* Equipment = FindEquipmentActor(Type);
	if (Equipment != nullptr)
	{
		if (IEquipable * Equipable = Cast<IEquipable>(Equipment))
		{
			OnUnequipped.Broadcast(Type, Equipment);
			IEquipable::Execute_Unequip(Equipment);
		}
		EquipmentMap[Type] = nullptr;
	}
}

AEquipmentBase* UEquipmentComponent::FindEquipmentActor(EEquipmentType Type)
{
	AEquipmentBase** FindResult = EquipmentMap.Find(Type);
	return (FindResult == nullptr) ? nullptr : *FindResult;
}

AWeaponBase* UEquipmentComponent::FindWeaponActor()
{
	AEquipmentBase* SwordEquipment = FindEquipmentActor(EEquipmentType::EET_Weapon);
	return Cast<AWeaponBase>(SwordEquipment);
}

void UEquipmentComponent::PullOutWeapons()
{
	if (AWeaponBase* LeftWeapon = FindWeaponActor())
	{
		LeftWeapon->PullOut();
	}
	if (AWeaponBase * RightWeapon = Cast<AWeaponBase>(FindEquipmentActor(EEquipmentType::EET_Shield)))
	{
		RightWeapon->PullOut();
	}
}

void UEquipmentComponent::PutInWeapons()
{
	if (AWeaponBase* LeftWeapon = FindWeaponActor())
	{
		LeftWeapon->PutIn();
	}
	if (AWeaponBase * RightWeapon = Cast<AWeaponBase>(FindEquipmentActor(EEquipmentType::EET_Shield)))
	{
		RightWeapon->PutIn();
	}
}
void UEquipmentComponent::DestroyAllEquipments()
{
	for (auto& Iter : EquipmentMap)
	{
		if (Iter.Value != nullptr)
		{
			Iter.Value->Destroy();
		}
	}
}
