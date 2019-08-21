// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API AEnemyControllerBase : public AAIController
{
	GENERATED_BODY()

protected : 
	AEnemyControllerBase();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UAIPerceptionComponent* AIPerceptionComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

public :
	virtual ETeamAttitude::Type	GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION(BlueprintCallable, Category = "NPCController")
	void SetEnemy(AActor* NewEnemy);

	void UpdateSightConfig(float VisionAngleDegrees, float Radius, float LoseRadius);
	//void UpdateHearingConfig();

protected :
	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCController")
	uint8 FactionID;

	UPROPERTY(BlueprintReadWrite, Category="NPCController")
	AActor* TargetEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCController")
	float FightDistance{600.0f};
	
};
