// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GlobalEventHandlerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERTHESTORM_API UGlobalEventHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGlobalEventHandlerComponent();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdatedEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerStatusUpdatedEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyStatusUpdatedEvent, const AActor*, EnemyToUpdate);

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "Global Event")
	FInventoryUpdatedEvent OnInventoryUpdated;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "Global Event")
	FPlayerStatusUpdatedEvent OnPLayerStatusUpdatedEvent;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "Global Event")
	FEnemyStatusUpdatedEvent OnEnemyStatusUpdatedEvent;

public:	
		
};
