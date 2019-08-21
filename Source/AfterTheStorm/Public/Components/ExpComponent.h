// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExpComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERTHESTORM_API UExpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UExpComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, Category="Exp")
	void AddExp(int32 Amount);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLevelUpEvent, const int32, NewLevel, bool, bIsInitial);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FExpUpdatedEvent, const int32, CurrentExp, const int32, LevelUpExp);

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category="Exp")
	FLevelUpEvent OnLevelUp;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category="Exp")
	FExpUpdatedEvent OnExpUpdated;

	//Util Functions
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Exp")
	int32 GetLevel() { return Level; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Exp")
	int32 GetCurrentExp() { return CurrentExp; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Exp")
	int32 GetLevelUpExp() { return LevelUpExp; }

	UFUNCTION(BlueprintCallable, Category = "Exp")
	void SetLevel(int32 NewLevel) { Level = NewLevel;  }
	UFUNCTION(BlueprintCallable, Category = "Exp")
	void SetCurrentExp(int32 Value) { CurrentExp = Value; }
	UFUNCTION(BlueprintCallable, Category = "Exp")
	void SetLevelUpExp(int32 Value) { LevelUpExp = Value; }
	//===========================================================================//

protected :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Exp")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Exp")
	int32 CurrentExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Exp")
	int32 LevelUpExp;
};
