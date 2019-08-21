// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"
/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API AMyGameState : public AGameState
{
	GENERATED_BODY()

public :
	AMyGameState();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GloalEvent")
	class UGlobalEventHandlerComponent* GlobalEventHandlerComponent;

protected:
};
