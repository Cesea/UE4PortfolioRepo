// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInstanceEnums.generated.h"


UENUM(BlueprintType)
enum class EGameState : uint8
{
	EGS_Invalid			UMETA(Hidden), 
	EGS_Main			UMETA(DisplayName="Main"),
	EGS_Creation		UMETA(DisplayName="Creation"),
	EGS_Loading			UMETA(DisplayName="Loading"),
	EGS_FullOptions		UMETA(DisplayName="FullOptions"),
	EGS_Game			UMETA(DisplayName="Game")
};
