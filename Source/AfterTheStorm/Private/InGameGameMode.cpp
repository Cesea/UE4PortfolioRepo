// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#include "Public/ASGameInstance.h"
#include "Public/Utils/ASSaveGame.h"

#include "Public/CharacterBase.h"
#include "Public/Actors/Players/PlayerCharacter.h"

#include "Public/Actors/NPCs/NPCBase.h"


void AInGameGameMode::BeginPlay()
{

}

void AInGameGameMode::SaveGame()
{
	UASGameInstance* GameInstance = (UASGameInstance*)UGameplayStatics::GetGameInstance((UObject* )GetWorld());
	if (UASSaveGame* GameStatus = GameInstance->GetCurrentGameStatus())
	{
		//Save Map Name
		FString MapName = GetWorld()->GetMapName();
		MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
		GameStatus->MapName = MapName;

		if (APlayerCharacter * PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			PlayerCharacter->SaveCharacter(GameStatus);
		}

		TArray<AActor*> NPCs;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANPCBase::StaticClass(), NPCs);
		for (const auto& NPC : NPCs)
		{
			if (ANPCBase * CastedNPC = Cast<ANPCBase>(NPC))
			{
				CastedNPC->SaveNPC(GameStatus);
			}
		}
		GameStatus->bIsFirstSave = false;
		UGameplayStatics::SaveGameToSlot(GameStatus, GameStatus->Name, 0);
	}
}
