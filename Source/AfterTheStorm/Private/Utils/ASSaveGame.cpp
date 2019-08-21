// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Utils/ASSaveGame.h"



UASSaveGame::UASSaveGame()
{
	PlayerLevel = 1;
	PlayerExp = 0;

	SavedTime = FDateTime::Now();

	PlayedTime = 0.0f;

	bIsFirstSave = true;
}

FString UASSaveGame::GetSavedTimeString()
{
	return SavedTime.ToString();
}