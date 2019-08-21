// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Public/Interfaces/Interactable.h"

#include "InteractableBase.generated.h"

UCLASS()
class AFTERTHESTORM_API AInteractableBase : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Interact_Implementation(AActor* Actor) override;
	virtual void ShowFeedbackWidget_Implementation(AActor* Actor) override;
	virtual void HideFeedbackWidget_Implementation(AActor* Actor) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactable")
	class UBoxComponent* CollisionBoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactable")
	class USphereComponent* InteractionSphereComponent;

	//Enable Sphere Collision
	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void EnableCollisions();
	//Disable Sphere Collision
	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void DisableCollisions();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable")
	FText MessageToShow;
};
