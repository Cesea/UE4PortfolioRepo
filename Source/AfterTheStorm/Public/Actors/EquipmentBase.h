// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Public/Actors/PickupBase.h"
#include "Public/Interfaces/Equipable.h"

#include "Public/Enums/CharacterEnums.h"
#include "Public/Enums/ItemEnums.h"

#include "EquipmentBase.generated.h"


USTRUCT(BlueprintType)
struct FEquipmentData : public FTableRowBase 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	EEquipmentType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	int Attribute;
};

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	EWeaponType Type;
};



/*AEquipmentBase*/
UCLASS()
class AFTERTHESTORM_API AEquipmentBase : public APickupBase, public IEquipable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquipmentBase();
protected :
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public :
	//IEquipable Implementations
	virtual void Equip_Implementation(class USkeletalMeshComponent* CharacterMesh) override;
	virtual void Unequip_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment")
	int32 GetAttribute();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment")
	FORCEINLINE EEquipmentType GetEquipmentType() { return EquipmentData->Type; }

	FORCEINLINE void SetIndexInInventory(int32 Index) { IndexInInventory = Index; }
	FORCEINLINE int32 GetIndexInInventory() { return IndexInInventory; }

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment")
	EEquipmentType GetEquipmentTypeFromDB();

	class USkeletalMeshComponent* EquippedMesh;

	UPROPERTY()
	int32 IndexInInventory;

	FEquipmentData* EquipmentData;
};

UCLASS()
class AFTERTHESTORM_API AWeaponBase : public AEquipmentBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public :
	virtual void Equip_Implementation(class USkeletalMeshComponent* CharacterMesh) override;
	virtual void Unequip_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PullOut();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PutIn();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	UAttackMove* Attack(EInputBufferKey InputBufferKey);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ResetCombo();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon")
	FORCEINLINE class UAttackRangeInfo* GetRangeInfo() { return RangeInfo; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon")
	FORCEINLINE EWeaponType GetWeaponType() { return WeaponData->Type; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon")
	FORCEINLINE class UParticleSystem* GetParryParticle() { return ParryParticle; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Weapon")
	FORCEINLINE class USoundBase* GetParrySound() { return ParrySound; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|Range")
	float GetShortRange();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon|Range")
	float GetMediumRange();


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class UWeaponTraceComponent* WeaponTraceComponent;

protected :
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	class UAttackMove* InitialAttackState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class UAttackMove* CurrentAttackState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects|Sound")
	class UParticleSystem* ParryParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects|Sound")
	class USoundBase* ParrySound;

	//Range Info
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	class UAttackRangeInfo* RangeInfo{nullptr};
	//==================================================================================//

	FWeaponData* WeaponData{nullptr};
};

