// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "InGameGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API AInGameGameMode : public AGameMode
{
	GENERATED_BODY()

public :
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SaveGame();

};
