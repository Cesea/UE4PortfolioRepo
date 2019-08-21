// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/ASGameInstance.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "MoviePlayer.h"

#include "Engine/World.h"

#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"

#include "Public/CharacterCreation/CreationOption.h"
#include "Public/Utils/ASSaveGame.h"
#include "Public/Utils/CharacterListSaveGame.h"

#include "Public/Components/StatComponent.h"
#include "Public/Components/ExpComponent.h"

UASGameInstance::UASGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> LoadedItemDB(TEXT("DataTable'/Game/CS_Work/Databases/DB_Items.DB_Items'"));
	ItemDB = LoadedItemDB.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> LoadedEquipmentDB(TEXT("DataTable'/Game/CS_Work/Databases/DB_Equipments.DB_Equipments'"));
	EquipmentDB = LoadedEquipmentDB.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> LoadedWeaponDB(TEXT("DataTable'/Game/CS_Work/Databases/DB_Weapons.DB_Weapons'"));
	WeaponDB = LoadedWeaponDB.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> LoadedEnemyDB(TEXT("DataTable'/Game/CS_Work/Databases/DB_Enemys.DB_Enemys'"));
	EnemyDB = LoadedEnemyDB.Object;


	static ConstructorHelpers::FObjectFinder<USkeletalMeshCreationOption> LoadedHeadCreationOptions(TEXT("SkeletalMeshCreationOption'/Game/CS_Work/Databases/HeadOptions.HeadOptions'"));
	HeadCreationOption = LoadedHeadCreationOptions.Object;
	static ConstructorHelpers::FObjectFinder<USkeletalMeshCreationOption> LoadedHairCreationOptions(TEXT("SkeletalMeshCreationOption'/Game/CS_Work/Databases/HairOptions.HairOptions'"));
	HairCreationOptions = LoadedHairCreationOptions.Object;
	static ConstructorHelpers::FObjectFinder<USkeletalMeshCreationOption> LoadedFaceHairCreationOptions(TEXT("SkeletalMeshCreationOption'/Game/CS_Work/Databases/FaceHairOptions.FaceHairOptions'"));
	FaceHairCreationOption = LoadedFaceHairCreationOptions.Object;
	static ConstructorHelpers::FObjectFinder<UTextureCreationOption> LoadedWarpaintCreationOptions(TEXT("TextureCreationOption'/Game/CS_Work/Databases/WarPaintOptions.WarPaintOptions'"));
	WarpaintCreationOption = LoadedWarpaintCreationOptions.Object;

	GameState = EGameState::EGS_Main;

	CurrentIndex = -1;
}

void UASGameInstance::QuitGame()
{
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), "Exit");
}

void UASGameInstance::Init()
{
	Super::Init();
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UASGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UASGameInstance::EndLoadingScreen);
}

void UASGameInstance::BeginLoadingScreen(const FString& MapName)
{
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
}

void UASGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
	UE_LOG(LogTemp, Warning, TEXT("End LoadingScreen"));
}

void UASGameInstance::ContinueGame(int32 Index)
{
	if (Index < CharacterListSave->CharacterList.Num())
	{
		CurrentIndex = Index;

		CurrentGameStatus = (UASSaveGame*)UGameplayStatics::LoadGameFromSlot(GetSlotName(Index), 0);

		UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentGameStatus->MapName));
	}
}

void UASGameInstance::ToMainLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Main");
}

FString UASGameInstance::GetSlotName(int32 Index)
{
	if (CharacterListSave != nullptr && Index < CharacterListSave->CharacterList.Num())
	{
		return CharacterListSave->CharacterList[Index];
	}
	return FString();
}

TArray<class UASSaveGame*> UASGameInstance::GetAllSaveGames()
{
	TArray<UASSaveGame*> Result;
	if (CharacterListSave != nullptr)
	{
		for (const FString& SlotName : CharacterListSave->CharacterList)
		{
			if (UASSaveGame * CastedSaveGame = Cast<UASSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0)))
			{
				Result.Add(CastedSaveGame);
			}
		}
	}
	return Result;
}

void UASGameInstance::DeleteSaveGame(int32 Index)
{
	if (CharacterListSave != nullptr && 
		Index < CharacterListSave->CharacterList.Num())
	{
		UGameplayStatics::DeleteGameInSlot(CharacterListSave->CharacterList[Index], 0);
		CharacterListSave->CharacterList.RemoveAt(Index);
		UGameplayStatics::SaveGameToSlot(CharacterListSave, "CharacterList", 0);
	}
}

void UASGameInstance::DeleteAllSaveGame()
{
	for (int i = CharacterListSave->CharacterList.Num() - 1; i >= 0; --i)
	{
		DeleteSaveGame(i);
	}
}

int32 UASGameInstance::NumSaveGameExist()
{
	if (CharacterListSave != nullptr)
	{
		return CharacterListSave->CharacterList.Num();
	}
	return -1;
}

bool UASGameInstance::IsGameExist(FString Name)
{
	if (CharacterListSave != nullptr)
	{
		return CharacterListSave->CharacterList.Contains(Name);
	}
	return false;
}

void UASGameInstance::TempSavePlayerData(UInventoryComponent* Inventory, UStatComponent* Health, UStatComponent* Stamina, UExpComponent* Exp)
{
	PlayerInventory = Inventory->GetInventories();
	PlayerGold = Inventory->GetGold();

	PlayerCurrentHealth = Health->GetCurrentValue();
	PlayerMaxHealth = Health->GetMaxValue();

	PlayerCurrentStamina = Stamina->GetCurrentValue();
	PlayerMaxStamina = Stamina->GetMaxValue();

	PlayerCurrentExp = Exp->GetCurrentExp();
	PlayerLevelUpExp = Exp->GetLevelUpExp();
	PlayerLevel = Exp->GetLevel();
}
