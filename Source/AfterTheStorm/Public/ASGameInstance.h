// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Public/Enums/GameInstanceEnums.h"

#include "Public/Components/InventoryComponent.h"

#include "ASGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API UASGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UASGameInstance();

public :
	UFUNCTION(BlueprintCallable, Category = "GameInstance")
	void QuitGame();

	virtual void Init() override;

	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION()
	virtual void EndLoadingScreen(UWorld* InLoadedWorld);

public :
	FORCEINLINE class UDataTable* GetItemDB() const { return ItemDB; }
	FORCEINLINE class UDataTable* GetEquipmentDB() const { return EquipmentDB; }
	FORCEINLINE class UDataTable* GetWeaponDB() const { return WeaponDB; }
	FORCEINLINE class UDataTable* GetEnemyDB() const { return EnemyDB; }

	FORCEINLINE class USkeletalMeshCreationOption* GetHeadOptions() const { return HeadCreationOption; }
	FORCEINLINE class USkeletalMeshCreationOption* GetHairOptions() const { return HairCreationOptions; }
	FORCEINLINE class USkeletalMeshCreationOption* GetFaceHairOptions() const { return FaceHairCreationOption; }
	FORCEINLINE class UTextureCreationOption* GetWarpaintOptions() const { return WarpaintCreationOption; }

	FORCEINLINE class UASSaveGame* GetCurrentGameStatus() const { return CurrentGameStatus; }

	UFUNCTION(BlueprintCallable, Category = "GameInstance|Save&Load")
	void ContinueGame(int32 Index);
	UFUNCTION(BlueprintCallable)
	void ToMainLevel();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GameInstance")
	FORCEINLINE EGameState GetGameState() { return GameState; }

	UFUNCTION(BlueprintCallable, Category="GameInstance")
	void SetGameState(EGameState NewState) { GameState = NewState; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameInstance")
	FString GetSlotName(int32 Index);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameInstance|Save&Load")
	TArray<class UASSaveGame*> GetAllSaveGames();

	UFUNCTION(BlueprintCallable, Category = "GameInstance|Save&Load")
	void DeleteSaveGame(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "GameInstance|Sav&Loade")
	void DeleteAllSaveGame();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameInstance|Utils")
	int32 NumSaveGameExist();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameInstance|Utils")
	bool IsGameExist(FString Name);

	FORCEINLINE int32 GetCurrentIndex() { return CurrentIndex; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GameInstance|Utils")
	FString GetCurrentSlotName() { return GetSlotName(GetCurrentIndex()); }

	UFUNCTION(BlueprintCallable)
	void TempSavePlayerData(UInventoryComponent* Inventory, class UStatComponent* Health, class UStatComponent* Stamina, class UExpComponent* Exp);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameInstance")
	EGameState GameState ;

	UPROPERTY(BlueprintReadWrite, Category = "GameInstance|Sav&Loade")
	int32 CurrentIndex;

	UPROPERTY(BlueprintReadWrite, Category = "GameInstance|Sav&Loade")
	class UCharacterListSaveGame* CharacterListSave;

	UPROPERTY(BlueprintReadWrite, Category = "GameInstance|Save&Load")
	class UASSaveGame* CurrentGameStatus;

	// DataTable for Items
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Databases")
	class UDataTable* ItemDB;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Databases")
	class UDataTable* EquipmentDB;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Databases")
	class UDataTable* WeaponDB;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Databases")
	class UDataTable* EnemyDB;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterCreation")
	class USkeletalMeshCreationOption* HeadCreationOption;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterCreation")
	class USkeletalMeshCreationOption* HairCreationOptions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterCreation")
	class USkeletalMeshCreationOption* FaceHairCreationOption;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterCreation")
	class UTextureCreationOption* WarpaintCreationOption;

public :
	UPROPERTY()
	TArray<FInventoryItem> PlayerInventory;
	UPROPERTY()
	int32 PlayerGold;

	UPROPERTY()
	float PlayerCurrentHealth;
	UPROPERTY()
	float PlayerMaxHealth;
	UPROPERTY()
	float PlayerCurrentStamina;
	UPROPERTY()
	float PlayerMaxStamina;

	UPROPERTY()
	int32 PlayerLevel;
	UPROPERTY()
	int32 PlayerCurrentExp;
	UPROPERTY()
	int32 PlayerLevelUpExp;



};