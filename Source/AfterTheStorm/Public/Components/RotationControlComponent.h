// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotationControlComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERTHESTORM_API URotationControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URotationControlComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//UFUNCTION(BlueprintCallable, Category = "RotationControl")
	//void LookAt(FVector Location);

	UFUNCTION(BlueprintCallable, Category = "RotationControl")
	void RotateTowardsDirection(FVector Direction);

	UFUNCTION(BlueprintCallable, Category = "RotationControl")
	void RotateTowardsActor(AActor* NewTarget);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RotationControl")
	bool IsInterpolating() { return ElapsedTime > 0.0f; }

protected :
	// 1 means can rotate 180 degree in one seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RotationControl")
	float RotationSpeed;

	float ElapsedTime;

	float EstimatedTime;

	FVector TargetDirection;
	AActor* TargetActor;
};
