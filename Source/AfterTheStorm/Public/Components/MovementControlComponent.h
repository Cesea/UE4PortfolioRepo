// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Public/Enums/CharacterEnums.h"
#include "MovementControlComponent.generated.h"

USTRUCT(BlueprintType)
struct FMovementSpeed
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed{ 180.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RunSpeed{ 450.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed{ 600.0f };
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERTHESTORM_API UMovementControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMovementControlComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MovementControl")
	EMovementState GetMovementState() { return MovementState; };
	UFUNCTION(BlueprintCallable, Category = "MovementControl")
	void SetMovementState(EMovementState NewState);

protected :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementControl")
	FMovementSpeed MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MovementControl")
	EMovementState MovementState;

	UPROPERTY()
	class UCharacterMovementComponent* CharacterMovementComponentRef;

};
