// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/NPCs/NPCBase.h"
#include "BossBase.generated.h"

/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API ABossBase : public ANPCBase
{
	GENERATED_BODY()

public : 

	virtual bool CanBeStuned() override;
	
};
