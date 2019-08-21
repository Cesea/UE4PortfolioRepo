// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillActorBase.generated.h"

UCLASS()
class AFTERTHESTORM_API ASkillActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillActorBase();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "SkillActor")
	void DamageActor(AActor* ActorToDamage);

protected :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillActor")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillActor")
	TArray<AActor*> DamagedActors;

};