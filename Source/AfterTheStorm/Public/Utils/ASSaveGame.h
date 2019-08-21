// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Public/Components/InventoryComponent.h"
#include "Public/Components/CustomizingComponent.h"
#include "ASSaveGame.generated.h"

USTRUCT(BlueprintType)
struct AFTERTHESTORM_API FCharacterInventorySaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FInventoryItem> InventoryItems;

	UPROPERTY(BlueprintReadWrite)
	int32 Gold;
};

USTRUCT(BlueprintType)
struct AFTERTHESTORM_API FCharacterStatSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float CurrentHealth{100.0f};
	UPROPERTY(BlueprintReadWrite)
	float MaxHealth{100.0f};

	UPROPERTY(BlueprintReadWrite)
	float CurrentStamina{100.0f};
	UPROPERTY(BlueprintReadWrite)
	float MaxStamina{100.0f};
};

USTRUCT(BlueprintType)
struct AFTERTHESTORM_API FCharacterSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(BlueprintReadWrite)
	FCharacterInventorySaveData InventorySave;

	UPROPERTY(BlueprintReadWrite)
	FCharacterStatSaveData StatSave;

	UPROPERTY(BlueprintReadWrite)
	bool bIsDead;
};

/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API UASSaveGame : public USaveGame
{
	GENERATED_BODY()

	UASSaveGame();

public :
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetSavedTimeString();

public :
	UPROPERTY(BlueprintReadWrite)
	FString Name;

	UPROPERTY(BlueprintReadWrite)
	FString MapName {"Field"};

	UPROPERTY(BlueprintReadWrite)
	int32 PlayerLevel {1};
	UPROPERTY(BlueprintReadWrite)
	int32 PlayerExp {0};
	UPROPERTY(BlueprintReadWrite)
	int32 PlayerLevelUpExp {100};

	UPROPERTY(BlueprintReadWrite)
	FCharacterSaveData PlayerSaveData;
	UPROPERTY(BlueprintReadWrite)
	FCharacterCustomizingInfo PlayerCustomizingInfo;

	UPROPERTY(BlueprintReadWrite)
	TMap<FName, FCharacterSaveData> NPCsSaveData;

	UPROPERTY(BlueprintReadWrite)
	FDateTime SavedTime;

	UPROPERTY(BlueprintReadWrite)
	float PlayedTime;

	UPROPERTY(BlueprintReadWrite)
	uint32 bIsFirstSave : 1;

	UPROPERTY(BlueprintReadWrite)
	uint32 bGotStartGears : 1;
};
