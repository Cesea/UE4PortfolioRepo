// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponTraceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERTHESTORM_API UWeaponTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponTraceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public :
	UFUNCTION(BlueprintCallable, Category = "Weapon Trace")
	void SetupWeaponTrace(class USkeletalMeshComponent* WeaponMesh, TArray<FName> Sockets);

	UFUNCTION(BlueprintCallable, Category = "Weapon Trace")
	void ActivateCollision();

	UFUNCTION(BlueprintCallable, Category = "Weapon Trace")
	void DeactivateCollision();

	UFUNCTION(BlueprintCallable, Category = "Weapon Trace")
	void UpdateLastSocketLocations();

	UFUNCTION(BlueprintCallable, Category = "Weapon Trace")
	void PerformTrace();

public :
	UPROPERTY(EditAnywhere)
	bool bDrawDebugLines;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> ClassesToIgnore;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AActor*> ActorsToIgnore;

	DECLARE_DYNAMIC_DELEGATE_OneParam(FWeaponHitEvent, const FHitResult&, HitResult);

	FWeaponHitEvent OnWeaponHitEvent;

protected :
	bool IsIgnoredClass(TSubclassOf<AActor> TestClass);
	bool IsIgnoredActor(AActor* Actor);

protected :
	UPROPERTY()
	class USkeletalMeshComponent* Mesh;

	UPROPERTY()
	TArray<FName> SocketNames;

	UPROPERTY()
	bool bShouldTrace;

	UPROPERTY()
	bool bShouldUpdateSocketLocations;

	UPROPERTY()
	TMap<FName, FVector> LastSocketLocations;

	UPROPERTY()
	TArray<AActor*> ActorsHit;

};
