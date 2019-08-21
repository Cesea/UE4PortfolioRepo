// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CharacterListSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API UCharacterListSaveGame : public USaveGame
{
	GENERATED_BODY()

public :
public : 
	UPROPERTY(BlueprintReadWrite, Category="CharacterList")
	TArray<FString> CharacterList;
};