// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public/Actors/MultiPartCharacterBase.h"
#include "Public/Interfaces/Interactable.h"
#include "Engine/DataTable.h"
#include "EnemyBase.generated.h"

USTRUCT(BlueprintType)
struct FEnemyData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyData")
	FName Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyData")
	FText Name {FText::FromString("TempName")};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyData")
	float Health{100};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyData")
	float Stamina{100};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyData")
	int32 ExpAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyData")
	float VisionAngleInDegrees{90.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyData")
	float SightRadius{1000.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyData")
	float LoseSightRadius{1400.0f};
};

/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API AEnemyBase : public AMultiPartCharacterBase, public IInteractable
{
	GENERATED_BODY()

protected :
	AEnemyBase();
	virtual void BeginPlay() override;

public :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPatrolComponent* PatrolComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UAIRotationControlComponent* AIRotationControlComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPersonalityComponent* PersonalityComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWidgetComponent* TargetWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWidgetComponent* HealthWidgetComponent;

public : 
	virtual void OnEquipmentEquipped_Implementation(EEquipmentType EquipmentType, class AEquipmentBase* Equipment) override;
	virtual void OnEquipmentUnequipped_Implementation(EEquipmentType EquipmentType, class AEquipmentBase* Equipment) override;

public :

	//IInteractable Overrides
	virtual void Interact_Implementation(AActor* Actor) ;
	virtual void ShowFeedbackWidget_Implementation(AActor* Actor);
	virtual void HideFeedbackWidget_Implementation(AActor* Actor);
	//==============================================================================================//

	//Targetables Overrides
	virtual void OnTargeted_Implementation(AActor* LookingActor);
	virtual void OnReleasedTarget_Implementation(AActor* LookingActor);
	//==============================================================================================//

	virtual void Stun_Implementation(EMovementDirection Direction, bool bIsHeavy);
	virtual void OnBeingHitted_Implementation(const FHitResult& HitResult, AActor* Attacker, float Damage);
	virtual void OnDead_Implementation();
	virtual void ToggleWeapon_Implementation();

	//AI State Update Related
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="NPCBase")
	void OnAIStateEnter(EAIState NewState);
	virtual void OnAIStateEnter_Implementation(EAIState NewState);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="NPCBase")
	void OnAIStateExit(EAIState ExitState);
	virtual void OnAIStateExit_Implementation(EAIState ExitState);
	//==============================================================================================//

	//Stat Related
	virtual void OnHealthStatChanged_Implementation(float Current, float Max);
	virtual void OnHealthStatStateChanged_Implementation(EStatState NewState, float Percent);

	virtual void OnStaminaStatChanged_Implementation(float Current, float Max);
	virtual void OnStaminaStatStateChanged_Implementation(EStatState NewState, float Percent);
	//==============================================================================================//

	//Resets
	void HideHealthWidgets();
	//==============================================================================================//

	//In Game evade will be called after query(After TargetLocation Updated)
	virtual FVector GetEvadeDirection() override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NPCBase")
	FORCEINLINE FName GetID() { return ID; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="NPCBase")
	FORCEINLINE FText GetName() { return EnemyData->Name; }

protected :

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FName ID;

	FEnemyData* EnemyData;

	FTimerHandle HideInfoWidgetsTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FText MessageToShow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	uint32 bIsTargeted : 1;

	UPROPERTY(BlueprintReadOnly, Category = "NPC")
	class AEnemyControllerBase* ControllerRef;
	
};
