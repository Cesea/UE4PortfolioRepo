// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Public/Enums/CharacterEnums.h"
#include "Public/Enums/ItemEnums.h"

#include "ASFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API UASFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	static class USM_InputAtom* AttackInputs[];

	UASFunctionLibrary();

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI")
	static EInputBufferKey AIActionToInputBufferKey(EAIAction Action);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	static FName GetSocketNameFromEquipmentType(EEquipmentType Type);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	static FName GetSheathSocketNameFromEquipmentType(EEquipmentType Type);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	static class USM_InputAtom* GetAttackInputFromInputKey(EInputBufferKey InputBufferKey);

};
