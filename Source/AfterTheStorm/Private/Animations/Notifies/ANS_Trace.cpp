// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Animations/Notifies/ANS_Trace.h"

#include "Components/SkeletalMeshComponent.h"
#include "Public/Components/WeaponTraceComponent.h"
#include "Public/Components/EquipmentComponent.h"

#include "Public/Actors/EquipmentBase.h"

FString UANS_Trace::GetNotifyName_Implementation() const
{
	return FString("Trace");
}

void UANS_Trace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Owner != nullptr)
	{
		UWeaponTraceComponent* WeaponTraceComp = Owner->FindComponentByClass<UWeaponTraceComponent>();
		if (WeaponTraceComp != nullptr)
		{
			WeaponTraceComp->ActivateCollision();
		}
		else
		{
			UEquipmentComponent* EquipmentComponent = MeshComp->GetOwner()->FindComponentByClass<UEquipmentComponent>();
			if (EquipmentComponent != nullptr)
			{
				AEquipmentBase* RightHandEquipment = EquipmentComponent->FindEquipmentActor(EEquipmentType::EET_Weapon);
				if (RightHandEquipment != nullptr)
				{
					WeaponTraceComp = RightHandEquipment->FindComponentByClass<UWeaponTraceComponent>();
					if (WeaponTraceComp != nullptr)
					{
						WeaponTraceComp->ActivateCollision();
					}
				}
			}
		}
	}
}

void UANS_Trace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Owner != nullptr)
	{
		UWeaponTraceComponent* WeaponTraceComp = Owner->FindComponentByClass<UWeaponTraceComponent>();
		if (WeaponTraceComp != nullptr)
		{
			WeaponTraceComp->DeactivateCollision();
		}
		else
		{
			UEquipmentComponent* EquipmentComponent = MeshComp->GetOwner()->FindComponentByClass<UEquipmentComponent>();
			if (EquipmentComponent != nullptr)
			{
				AEquipmentBase* RightHandEquipment = EquipmentComponent->FindEquipmentActor(EEquipmentType::EET_Weapon);
				if (RightHandEquipment != nullptr)
				{
					WeaponTraceComp = RightHandEquipment->FindComponentByClass<UWeaponTraceComponent>();
					if (WeaponTraceComp != nullptr)
					{
						WeaponTraceComp->DeactivateCollision();
					}
				}
			}
		}
	}
}
