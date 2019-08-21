// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERTHESTORM_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public :
	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void AddInteractable(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void RemoveInteractable(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void Interact();

private :
	int FindFocusedActorIndex();

	void UpdateFocusedIndex(int NewIndex);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable")
	TArray<AActor*> InteractableActors;

private :
	int FocusedIndex;

};
