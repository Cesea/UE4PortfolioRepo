// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Public/Enums/CharacterEnums.h"
#include "Public/Enums/ItemEnums.h"
#include "EquipmentComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERTHESTORM_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquippedEvent, const EEquipmentType, EquipmentType, class AEquipmentBase*, Equipment);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUnequippedEvent, const EEquipmentType, EquipmentType, class AEquipmentBase*, Equipment);

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FEquippedEvent OnEquipped;
	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FUnequippedEvent OnUnequipped;

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void Equip(EEquipmentType Type, TSubclassOf<class AEquipmentBase> EquipmentClass, int32 IndexInInventory);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void Unequip(EEquipmentType Type);

	//Util Functions
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void SetEquipmentClass(EEquipmentType Type, TSubclassOf<class AEquipmentBase> Class) { EquipmentClassMap.Add(Type, Class); }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment")
	TSubclassOf<AEquipmentBase> GetEquipmentClass(EEquipmentType Type) { return *EquipmentClassMap.Find(Type); }

	//Util Functions
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void SetEquipmentActor(EEquipmentType Type, class AEquipmentBase* Equipment) { EquipmentMap.Add(Type, Equipment); }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment")
	class AEquipmentBase* FindEquipmentActor(EEquipmentType Type);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment")
	bool IsAnythingEquipped(EEquipmentType Type) { return FindEquipmentActor(Type) != nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment")
	class AWeaponBase* FindWeaponActor();

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void PullOutWeapons();

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void PutInWeapons();

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void DestroyAllEquipments();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	TMap<EEquipmentType, TSubclassOf<class AEquipmentBase>> EquipmentClassMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	TMap<EEquipmentType, AEquipmentBase*> EquipmentMap;

	UPROPERTY()
	class USkeletalMeshComponent* SkeletalMeshComponentRef;

};