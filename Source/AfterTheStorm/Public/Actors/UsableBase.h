// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public/Actors/PickupBase.h"

#include "Public/Interfaces/Usable.h"

#include "UsableBase.generated.h"




UCLASS()
class AFTERTHESTORM_API AUsableBase : public APickupBase, public IUsable 
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUsableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Use_Implementation(AActor* Actor) override;

	FORCEINLINE EEffectTarget GetEffectTarget() { return EffectTarget; }
	FORCEINLINE EEffectType GetEffectType() { return EffectType; }

protected : 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Usable")
	EEffectTarget EffectTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Usable")
	EEffectType EffectType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Usable")
	float EffectAmount;
};
