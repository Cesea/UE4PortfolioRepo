// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Public/Enums/CharacterEnums.h"
#include "StatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERTHESTORM_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void InitializeValues(float NewCurrent, float NewMax);


	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Delegates
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStatChangedEvent, const float, CurrentStat, const float, MaxStat);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStatZeroEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStatStateChangedEvent, const EStatState, NewState, const float, Percent);

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FStatChangedEvent OnStatChanged;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FStatZeroEvent OnStatZeroEvent;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FStatStateChangedEvent OnStatStateChanged;
	//======================================================================================//


	UFUNCTION(BlueprintCallable, Category = "Stat")
	void AddStat(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Stat")
	void SubStat(float Amount);

	//Util Functions
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Stat")
	float GetCurrentValue() { return Current; }
	UFUNCTION(BlueprintCallable, Category = "Stat")
	void SetCurrentValue(float NewValue);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Stat")
	float GetMaxValue() { return Max; }
	UFUNCTION(BlueprintCallable, Category = "Stat")
	void SetMaxValue(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "Stat|Regen")
	void SetRegenDelay(float NewDelay) { RegenDelaySeconds = NewDelay; }
	UFUNCTION(BlueprintCallable, Category = "Stat|Regen")
	void SetRegenPerSeconds(float Regen) { RegenPerSeconds = Regen; }
	//================================================================================//

protected :
	void Regenerate();

	void UpdateStatState();

protected :	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Current;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Regen")
	float RegenDelaySeconds;

	float DelayAccum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|Regen")
	float RegenPerSeconds;

	EStatState PrevStatState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	EStatState StatState;

	FTimerHandle RegenTimerHandle;

	UPROPERTY()
	class UGlobalEventHandlerComponent* GlobalEventHandlerComponentRef;

	UPROPERTY()
	uint32 OwnedByPlayer : 1;
};
