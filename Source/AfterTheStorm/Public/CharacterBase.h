// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Public/Enums/CharacterEnums.h"
#include "Public/Enums/ItemEnums.h"
#include "Public/Interfaces/Attackable.h"
#include "Public/Interfaces/Targetable.h"
#include "CharacterBase.generated.h"


UCLASS()
class AFTERTHESTORM_API ACharacterBase : public ACharacter, public IAttackable, public ITargetable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called after components initialized
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* InteractionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UInputBufferComponent* InputBufferComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStateMachineComponent* StateMachineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UMovementControlComponent* MovementControlComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStatComponent* HealthStatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStatComponent* StaminaStatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UEquipmentComponent* EquipmentComponent;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//class UCustomizingComponent* CustomizingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UMontageManagerComponent* MontageManagerComponent;

public :
	//Interafaces Implementations
	/*Attackables*/
	virtual void HandleHit_Implementation(const FHitResult& HitResult, AActor* Attacker, float Damage) override;
	virtual void Impact_Implementation() override;
	virtual bool IsAlive_Implementation() override;

	virtual bool IsVulnerable() override { return bIsVulnerable; }
	virtual void SetVulnerable_Implementation(bool Value) override { bIsVulnerable = Value; }

	virtual bool GetIsDefensing() override { return bIsDefensing; }
	virtual void SetIsDefensing_Implementation(bool Value) override { bIsDefensing = Value; }

	virtual bool IsOtherTeam_Implementation(int32 TestID) override { return TestID != TeamId; }
	virtual bool IsHostile_Implementation() override { return bIsHostile; }
	virtual int32 GetTeamId_Implementation() override { return TeamId; }
	//==========================================================================================//

	/*Targetables*/
	virtual void OnTargeted_Implementation(AActor* LookingActor) override;
	virtual void OnReleasedTarget_Implementation(AActor* LookingActor) override;
	//==========================================================================================//

	//Virtual Functions Overrides
	virtual void OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal, const FVector& PreviousFloorContactNormal, const FVector& PreviousLocation, float TimeDelta) override;
	//==========================================================================================//

public :
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Actions")
	void MainAttack(EInputBufferKey InputKey);
	virtual void MainAttack_Implementation(EInputBufferKey InputKey);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Actions")
	void Defense();
	virtual void Defense_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Actions")
	void Stun(EMovementDirection Direction, bool bIsHeavy);
	virtual void Stun_Implementation(EMovementDirection Direction, bool bIsHeavy);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Actions")
	void Evade(FVector EvadeDirection);
	virtual void Evade_Implementation(FVector EvadeDirection);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Actions")
	void DoJump();
	virtual void DoJump_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Actions")
	void ToggleWeapon();
	virtual void ToggleWeapon_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Actions")
	void InteractWith();
	virtual void InteractWith_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnBeingHitted(const FHitResult& HitResult, AActor* Attacker, float Damage);
	virtual void OnBeingHitted_Implementation(const FHitResult& HitResult, AActor* Attacker, float Damage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnHitSuccess(const FHitResult& HitResult);
	virtual void OnHitSuccess_Implementation(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDefenseSuccess(AActor* Attacker);
	virtual void OnDefenseSuccess_Implementation(AActor* Attacker);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Life")
	void OnDead();
	virtual void OnDead_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEquipmentEquipped(EEquipmentType EquipmentType, class AEquipmentBase* Equipment);
	virtual void OnEquipmentEquipped_Implementation(EEquipmentType EquipmentType, class AEquipmentBase* Equipment) {}
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEquipmentUnequipped(EEquipmentType EquipmentType, class AEquipmentBase* Equipment);
	virtual void OnEquipmentUnequipped_Implementation(EEquipmentType EquipmentType, class AEquipmentBase* Equipment) {}


	UFUNCTION(BlueprintCallable, Category = "Actions")
	void StartSprint();
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void StopSprint();

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void ToggleWalkRun();

public :
	//Conditions
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Conditions")
	virtual bool CanAttack();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Conditions")
	virtual bool CanDoJump();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Conditions")
	virtual bool CanBeHitted(AActor* Attacker);

	//UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Conditions")
	virtual bool CanBeStuned();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Conditions")
	virtual bool CanEvade();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Conditions")
	virtual bool CanDefense();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Conditions")
	virtual bool CanToggleWeapon();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Conditions")
	virtual bool CanSprint();
	//==========================================================================================//


	//Resets
	virtual void ResetStatus();

	UFUNCTION(BlueprintCallable, Category = "Resets")
	void ResetCombat(bool ResetStatus);
	UFUNCTION(BlueprintCallable, Category = "Resets")
	void ResetJump();
	UFUNCTION(BlueprintCallable, Category = "Resets")
	void ResetRootmotion();
	UFUNCTION(BlueprintCallable, Category = "Resets")
	void ResetRootmotionMontageEnd(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable, Category = "Resets")
	void ResetAccumulatedDamage() { AccumulatedDamage = 0.0f; }
	//==========================================================================================//

	//RootMotions
	UFUNCTION(BlueprintCallable, Category = "Resets")
	void IgnoreRootmotion();
	UFUNCTION(BlueprintCallable, Category = "Rootmotions")
	void EnableRootmotion();
	//==========================================================================================//

	//Orientation
	UFUNCTION(BlueprintCallable, Category = "Orientations")
	void OrientToController();
	UFUNCTION(BlueprintCallable, Category = "Orientations")
	void OrientToMovement();
	//==========================================================================================//


	//InputBuffer Event Related
	UFUNCTION()
	virtual void OnInputBufferConsumed(enum EInputBufferKey InputKey);
	UFUNCTION()
	virtual void OnInputBufferClose();
	//==========================================================================================//

	//StateMachine Event Related
	UFUNCTION()
	virtual void OnStateEnd(enum EState State);
	UFUNCTION()
	virtual void OnStateEnter(enum EState State);
	//==========================================================================================//

	//Stat's Event Related
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stat|Health")
	void OnHealthStatChanged(float Current, float Max);
	virtual void OnHealthStatChanged_Implementation(float Current, float Max) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stat|Health")
	void OnHealthStatStateChanged(EStatState NewState, float Percent);
	virtual void OnHealthStatStateChanged_Implementation(EStatState NewState, float Percent) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stat|Stamina")
	void OnStaminaStatStateChanged(EStatState NewState, float Percent);
	virtual void OnStaminaStatStateChanged_Implementation(EStatState NewState, float Percent) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stat|Stamina")
	void OnStaminaStatChanged(float Current, float Max);
	virtual void OnStaminaStatChanged_Implementation(float Current, float Max) {}

	//==========================================================================================//


	//Interaction Sphere Related
	//InteractionSphere ComponentBeginOverlap
	UFUNCTION()
	void OnInteractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	//InteractionSphere ComponentEndOverlap
	UFUNCTION()
	void OnInteractionSphereEndOverlap(UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);
	//==========================================================================================//

	//WeaponTrace Event Related
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnWeaponHit(const FHitResult& HitResult);
	virtual void OnWeaponHit_Implementation(const FHitResult& HitResult);

	//Virtual Functions
	virtual FVector GetEvadeDirection();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual EMovementDirection GetStunDirection(FVector HitterLocation);
	//==========================================================================================//

	FORCEINLINE EStanceState GetStanceState() { return StanceState; }

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Attack")
	EAttackType AttackType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stance")
	EStanceState StanceState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects|Particles")
	UParticleSystem* BloodParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects|Particles")
	UParticleSystem* HitParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects|Sounds")
	class USoundBase* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rootmotion")
	FTimerHandle EnableRootmotionTimerHandle;

	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	FTimerHandle ResetAccumulatedDamageTimerHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Damage")
	float AccumulatedDamage{0.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageAccumulationLimit{35.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TeamId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rootmotion")
	float AddMovementScaleValue{0.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	uint32 bIsVulnerable : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	uint32 bIsDefensing : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	uint32 bIsWeaponToggling : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	uint32 bWasMovingInToggle : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	uint32 bWasAcceleratingOnToggleWeapon : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	uint32 bIsRootmotionIgnored : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bIsHostile : 1;
};