// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Public/Enums/ItemEnums.h"

#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	FInventoryItem()
		:ItemClass(nullptr), Count(0), bIsEquipped(0)
	{
	}

	FInventoryItem(TSubclassOf<class APickupBase> Class, int32 Amount)
		:ItemClass(Class), Count(Amount), bIsEquipped(0)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TSubclassOf<class APickupBase> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	uint32 bIsEquipped : 1;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERTHESTORM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	//If returns true, ui has to be updated
	UFUNCTION(BlueprintCallable, Category="Utils")
	void AddItem(TSubclassOf<class APickupBase> Pickup, int32 Amount);

	//If returns true, ui has to be updated
	UFUNCTION(BlueprintCallable, Category="Utils")
	void RemoveItem(int32 InventoryIndex, int32 Amount, bool SpawnOnWorld);

	//If returns true, ui has to be updated
	UFUNCTION(BlueprintCallable)
	bool OnInventoryItemInteract(int32 Index);

	//If returns true, ui has to be updated
	UFUNCTION(BlueprintCallable)
	void ExchangeItem(UInventoryComponent* OtherInventory, int32 Index, bool IsFree);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 GetBestWeaponIndex();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void EquipEquipments();

	UFUNCTION(BlueprintCallable, Category = "Inventory|Gold")
	void AddGold(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Inventory|Gold")
	void SubGold(int32 Amount);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 FindUsable(EEffectTarget EffectTarget, EEffectType EffectType);

	FORCEINLINE int32 GetGold() { return Gold; }
	FORCEINLINE void SetGold(int32 NewGold) { Gold = NewGold; }

	FORCEINLINE TArray<FInventoryItem> GetInventories() { return Inventories; }
	FORCEINLINE void SetInventory(TArray<FInventoryItem> NewInventories) { Inventories = NewInventories; }

protected:
	void ToggleItemEquipped(int32 Index);

	FORCEINLINE bool GetIsEquipped(int32 Index) { return Inventories[Index].bIsEquipped; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	TArray<int32> GetInventoryItemsIndexByType(EItemType Type);

	struct FItemData* FindItemData(int32 Index);

protected:
	class UASGameInstance* GameInstanceRef;
	class AMyGameState* GameStateRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventoryItem> Inventories;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Inventory|Gold")
	int32 Gold;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//int32 IntenvotrySlotLimit;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//int32 InventoryWeightLimit;

};
