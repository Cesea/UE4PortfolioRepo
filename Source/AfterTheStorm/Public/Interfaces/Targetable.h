// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Targetable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTargetable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AFTERTHESTORM_API ITargetable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//Passive Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Targetable")
	void OnTargeted(AActor* LookingActor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Targetable")
	void OnReleasedTarget(AActor* LookingActor);
	//=============================================================================//

	//Active Functions
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Targetable")
	//void OnTargetingSuccess(AActor* TargetedActor);
	////This Function called also when targeting End
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Targetable")
	//void OnTargetingFail();
	//=============================================================================//

};
