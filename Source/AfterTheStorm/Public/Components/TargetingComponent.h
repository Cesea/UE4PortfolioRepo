// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERTHESTORM_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	UTargetingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public : 
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:	
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void LockTarget();

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void ChangeTarget(FVector2D ScreenDirection);

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void UnlockTarget();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Targeting")
	bool IsTargeted() { return TargetedActor != nullptr; }

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void CheckTargetActor();

	FORCEINLINE AActor* GetTargetedActor() { return TargetedActor; }

protected :
	AActor* FindTargetActor();

	AActor* FindActorOnScreenDirection(FVector2D ScreenDirection);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Targeting")
	bool CheckVisibility(class AActor* Actor);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Targeting")
	bool CheckDistance(class AActor* Actor);

protected :

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Targeting")
	class AActor* TargetedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetSearchDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetSearchDegree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetLooseDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToQuery;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY()
	class APlayerController* ControllerRef;

	UPROPERTY()
	class APlayerCharacter* PlayerCharacterRef;

	UPROPERTY()
	FTimerHandle CheckTargetTimerHandle;

	float PrevInputDeltaLength;
};
