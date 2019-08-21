// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public/Actors/NPCs/EnemyBase.h"

#include "NPCBase.generated.h"

/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API ANPCBase : public AEnemyBase
{
	GENERATED_BODY()


protected :

	ANPCBase();

	virtual void BeginPlay() override;

public :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UConversationComponent* ConversationComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCustomizingComponent* CustomizingComponent;
public :
	//IInteractable Overrides
	virtual void Interact_Implementation(AActor* Actor) override;
	virtual void ShowFeedbackWidget_Implementation(AActor* Actor) override;
	virtual void HideFeedbackWidget_Implementation(AActor* Actor) override;
	//==============================================================================================//

	virtual void OnBeingHitted_Implementation(const FHitResult& HitResult, AActor* Attacker, float Damage) override;

	virtual void OnAIStateEnter_Implementation(EAIState NewState) override;
	virtual void OnAIStateExit_Implementation(EAIState ExitState) override;

	virtual void OnDead_Implementation() override;



	void SaveNPC(class UASSaveGame* SaveGame);
	void LoadNPC();

protected :
	
};