// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Public/Enums/CharacterEnums.h"
#include "StateMachineComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERTHESTORM_API UStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateMachineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="StateMachine")
	FORCEINLINE EState GetState() { return State; }

	UFUNCTION(BlueprintCallable, Category = "StateMachine")
	void SetState(EState NewState);

	DECLARE_EVENT_OneParam(UStateMachineComponent, FStateEndEvent, EState)
	DECLARE_EVENT_OneParam(UStateMachineComponent, FStateEnterEvent, EState)

	FStateEndEvent OnStateEndEvent;
	FStateEnterEvent OnStateEnterEvent;

private :

	UPROPERTY(VisibleAnywhere)
	EState State;
};
