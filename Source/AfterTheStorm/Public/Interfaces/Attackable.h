// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Attackable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttackable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AFTERTHESTORM_API IAttackable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attackable")
	void HandleHit(const FHitResult& HitResult, AActor* Attacker, float Damage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attackable")
	void Impact();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attackable")
	bool IsAlive();

	virtual bool IsVulnerable() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attackable")
	void SetVulnerable(bool Value);

	virtual bool GetIsDefensing() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attackable")
	void SetIsDefensing(bool Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attackable")
	bool IsOtherTeam(int32 TestID);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attackable")
	bool IsHostile();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attackable")
	int32 GetTeamId();
};
