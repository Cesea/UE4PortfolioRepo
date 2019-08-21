// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/InventoryComponent.h"

#include "Components/SkeletalMeshComponent.h"

#include "Engine/World.h"
#include "Public/ASGameInstance.h"
#include "Public/MyGameState.h"
#include "Public/Actors/PickupBase.h"
#include "Public/Actors/UsableBase.h"

#include "Public/Interfaces/Usable.h"
#include "Public/Interfaces/Equipable.h"

#include "Public/Components/EquipmentComponent.h"
#include "Public/Components/GlobalEventHandlerComponent.h"
#include "Public/Actors/EquipmentBase.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	if (UASGameInstance * GameInstance = Cast<UASGameInstance>(GetOwner()->GetGameInstance()))
	{
		GameInstanceRef = GameInstance;
	}
	if (AMyGameState * MyGameStatee = Cast<AMyGameState>(GetWorld()->GetGameState()))
	{
		GameStateRef = MyGameStatee;
	}
}

void UInventoryComponent::AddItem(TSubclassOf<class APickupBase> PickupClass, int32 Amount)
{
	FItemData* Data = GameInstanceRef->GetItemDB()->FindRow<FItemData>(PickupClass.GetDefaultObject()->GetID(), FString(""));

	if (Data != nullptr)
	{
		for (int i = 0; i < Inventories.Num(); ++i)
		{
			if (Inventories[i].ItemClass == PickupClass)
			{
				if (Data->bIsStackable)
				{
					Inventories[i].Count += Amount;
				}
				else
				{
					Inventories.Add(FInventoryItem(PickupClass, Amount));
				}
				GameStateRef->GlobalEventHandlerComponent->OnInventoryUpdated.Broadcast();
				return;
			}
		}

		Inventories.Add(FInventoryItem(PickupClass, Amount));
		GameStateRef->GlobalEventHandlerComponent->OnInventoryUpdated.Broadcast();
	}
}

void UInventoryComponent::RemoveItem(int32 InventoryIndex, int32 Amount, bool SpawnOnWorld)
{
	if (InventoryIndex >= Inventories.Num())
	{
		return;
	}

	bool bShouldUpdateInventory = false;
	FItemData* ItemData = FindItemData(InventoryIndex);
	if (ItemData != nullptr)
	{
		switch (ItemData->Type)
		{
		case EItemType::EIT_Weapon: 
		case EItemType::EIT_Armor: 
		{
			if (GetIsEquipped(InventoryIndex))
			{
				FName ID = Inventories[InventoryIndex].ItemClass.GetDefaultObject()->GetID();
				FEquipmentData* EquipmentData = GameInstanceRef->GetEquipmentDB()->FindRow<FEquipmentData>(ID, FString(""));
				UEquipmentComponent* EquipmentComponent = GetOwner()->FindComponentByClass<UEquipmentComponent>();
				if (EquipmentData != nullptr && EquipmentComponent != nullptr)
				{
					if (AEquipmentBase * EquipingActor = EquipmentComponent->FindEquipmentActor(EquipmentData->Type))
					{
						EquipmentComponent->Unequip(EquipmentData->Type);
						Inventories[InventoryIndex].bIsEquipped = false;
					}
					bShouldUpdateInventory = true;
				}
			}
			else
			{
				bShouldUpdateInventory = true;
			}
		}break;
		case EItemType::EIT_Usable:
		case EItemType::EIT_Quest: 
		case EItemType::EIT_Misc: 
		{
			bShouldUpdateInventory = true;
		}break;
		}
	}

	if (bShouldUpdateInventory)
	{
		if (SpawnOnWorld)
		{
			UWorld* World = GetWorld();
			if (World != nullptr)
			{
				FActorSpawnParameters SpawnParameters;
				SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				FTransform TransformToSpawn = GetOwner()->GetActorTransform();
				TransformToSpawn.SetLocation(TransformToSpawn.GetLocation() + (GetOwner()->GetActorForwardVector() * 50.0f));
				World->SpawnActor<APickupBase>(Inventories[InventoryIndex].ItemClass, TransformToSpawn, SpawnParameters);
			}
		}

		Inventories[InventoryIndex].Count -= Amount;
		if (Inventories[InventoryIndex].Count <= 0)
		{
			Inventories.RemoveAt(InventoryIndex);
		}
		GameStateRef->GlobalEventHandlerComponent->OnInventoryUpdated.Broadcast();
	}
}

bool UInventoryComponent::OnInventoryItemInteract(int32 Index)
{
	if (Index >= Inventories.Num())
	{
		return false;
	}

	FName ID = Inventories[Index].ItemClass.GetDefaultObject()->GetID();
	FItemData* Data = GameInstanceRef->GetItemDB()->FindRow<FItemData>(ID,  FString());

	switch (Data->Type)
	{
	case EItemType::EIT_Weapon:
	case EItemType::EIT_Armor:
	{
		FEquipmentData* EquipmentData = GameInstanceRef->GetEquipmentDB()->FindRow<FEquipmentData>(ID, FString(""));
		UEquipmentComponent* EquipmentComponent = GetOwner()->FindComponentByClass<UEquipmentComponent>();
		if (EquipmentData != nullptr)
		{
			if (EquipmentComponent != nullptr && Inventories[Index].ItemClass->IsChildOf<AEquipmentBase>())
			{
				TSubclassOf<AEquipmentBase> CastedEquipmentBase = *Inventories[Index].ItemClass;
				//if Equiping same type of weapon
				if (AEquipmentBase* EquipingActor = EquipmentComponent->FindEquipmentActor(EquipmentData->Type))
				{
					if (Index == EquipingActor->GetIndexInInventory())
					{
						EquipmentComponent->Unequip(EquipmentData->Type);
						Inventories[Index].bIsEquipped = false;
					}
					else
					{
						EquipmentComponent->Unequip(EquipmentData->Type);
						Inventories[EquipingActor->GetIndexInInventory()].bIsEquipped = false;

						EquipmentComponent->Equip(EquipmentData->Type, CastedEquipmentBase, Index);
						Inventories[Index].bIsEquipped = true;
					}
				}
				else
				{
					EquipmentComponent->Equip(EquipmentData->Type, CastedEquipmentBase, Index);
					Inventories[Index].bIsEquipped = true;

				}
				GameStateRef->GlobalEventHandlerComponent->OnInventoryUpdated.Broadcast();
				return true;
			}
		}
	} break;
	case EItemType::EIT_Usable:
	{
		if (IUsable * Usable = Cast<IUsable>(Inventories[Index].ItemClass.GetDefaultObject()))
		{
			IUsable::Execute_Use(Inventories[Index].ItemClass.GetDefaultObject(), GetOwner());

			RemoveItem(Index, 1, false);

			GameStateRef->GlobalEventHandlerComponent->OnInventoryUpdated.Broadcast();
			return true;
		}
	} break;
	default:
		break;
	}

	return false;
}

void UInventoryComponent::ToggleItemEquipped(int32 Index)
{
	Inventories[Index].bIsEquipped = !Inventories[Index].bIsEquipped;
}

TArray<int32> UInventoryComponent::GetInventoryItemsIndexByType(EItemType Type)
{
	TArray<int32> ResultArray;
	for (int i = 0; i < Inventories.Num(); ++i)
	{
		FItemData* Data = GameInstanceRef->GetItemDB()->FindRow<FItemData>(Inventories[i].ItemClass.GetDefaultObject()->GetID(), FString(""));
		if (Data != nullptr)
		{
			if (Type == Data->Type)
			{
				ResultArray.Emplace(i);
			}
		}
	}
	return ResultArray;
}


void UInventoryComponent::ExchangeItem(UInventoryComponent* OtherInventory, int32 Index, bool FreeCharge)
{
	if (Index >= Inventories.Num())
	{
		return;
	}
	const FInventoryItem& Item = Inventories[Index];

	if (!FreeCharge)
	{
		FItemData* Data = FindItemData(Index);

		int OtherGold = OtherInventory->Gold;
		//돈이 충분히 있다면
		if (OtherGold > Data->Value)
		{
			AddGold(Data->Value);
			OtherInventory->SubGold(Data->Value);
			OtherInventory->AddItem(Item.ItemClass, 1);
			RemoveItem(Index, 1, false);
		}
	}
	else
	{
		OtherInventory->AddItem(Item.ItemClass, 1);
		RemoveItem(Index, 1, false);
	}

	GameStateRef->GlobalEventHandlerComponent->OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::GetBestWeaponIndex()
{
	int32 BestAttribute = 0;
	int32 BestIndex = -1;
	TArray<int32> AllWeaponsIndex = GetInventoryItemsIndexByType(EItemType::EIT_Weapon);
	for (int i = 0; i < AllWeaponsIndex.Num(); ++i)
	{
		if (TSubclassOf<AWeaponBase> CastedWeapon = *Inventories[AllWeaponsIndex[i]].ItemClass)
		{
			FEquipmentData* EquipmentData = GameInstanceRef->GetEquipmentDB()->FindRow<FEquipmentData>(CastedWeapon.GetDefaultObject()->GetID(), "");
			if ((EquipmentData != nullptr) &&
				(EquipmentData->Type == EEquipmentType::EET_Weapon) &&
				(EquipmentData->Attribute > BestAttribute))
			{
				BestIndex = AllWeaponsIndex[i];
			}
		}
	}
	return BestIndex;
}

void UInventoryComponent::EquipEquipments()
{
	int32 Index = 0;
	for (const FInventoryItem& Item : Inventories)
	{
		if(Item.bIsEquipped)
		{
			OnInventoryItemInteract(Index);
		}
		Index++;
	}
}

void UInventoryComponent::AddGold(int32 Amount)
{
	Gold += Amount;
	if (Gold < 0)
	{
		Gold = 0;
	}
}

void UInventoryComponent::SubGold(int32 Amount)
{
	AddGold(-Amount);
}

int32 UInventoryComponent::FindUsable(EEffectTarget EffectTarget, EEffectType EffectType)
{
	for (int32 i = 0; i < Inventories.Num(); ++i)
	{
		if (IUsable * CastedUsable = Cast<IUsable>(Inventories[i].ItemClass.GetDefaultObject()))
		{
			TSubclassOf<AUsableBase> CastedUsableBase = *Inventories[i].ItemClass;

			if (EffectTarget == CastedUsableBase.GetDefaultObject()->GetEffectTarget() &&
				EffectType == CastedUsableBase.GetDefaultObject()->GetEffectType())
			{
				return i;
			}
		}
	}
	return -1;
}


FItemData* UInventoryComponent::FindItemData(int32 Index)
{
	FName ID = Inventories[Index].ItemClass.GetDefaultObject()->GetID();
	return  GameInstanceRef->GetItemDB()->FindRow<FItemData>(ID,  FString());
}
