// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERTHESTORM_API UPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPatrolComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	//Returns Next PatrolPoint's Location
	UFUNCTION(BlueprintCallable, Category = "PartrolComponent")
	FVector ToNextPatrolPoint();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolComponent")
	uint32 bMovingRandomly : 1;

	UPROPERTY(BlueprintReadWrite, Category = "PatrolComponent|Random")
	FVector SpawnedLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolComponent|Random")
	float MovingRadius{600};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PatrolComponent")
	TArray<class APatrolPoint*> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolComponent")
	int32 CurrentPatrolIndex;

};
