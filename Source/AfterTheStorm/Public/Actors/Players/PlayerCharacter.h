// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public/Actors/MultiPartCharacterBase.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class AFTERTHESTORM_API APlayerCharacter : public AMultiPartCharacterBase, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class URotationControlComponent* RotationControlComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* InteractionSphereParentComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UTargetingComponent* TargetingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UExpComponent* ExpComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCustomizingComponent* CustomizingComponent;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnLevelUp(int32 NewLevel, bool bIsInitial);
	void OnLevelUp_Implementation(int32 NewLevel, bool bIsInirial);

	UFUNCTION(BlueprintCallable, Category="Save&Load")
	void SaveCharacter(class UASSaveGame* SaveGame);
	UFUNCTION(BlueprintCallable, Category="Save&Load")
	void LoadCharacter();

	virtual void ResetStatus() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TraceDistance;

	//Movements InputValue
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float ForwardAxisValue;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float RightAxisValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float MinPitchValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float MaxPitchValue;

public :
	//Targetable Interface Related
	void OnTargetingSuccess(AActor* TargetedActor);
	void OnTargetingFail();
	//==============================================================================//

public:
	//For Player move forward
	void MoveForward(float Value);
	//For Player move right
	void MoveRight(float Value);
	//Turn camera around
	void Turn(float Value);
	//Look camera up and down
	void LookUp(float Value);

	//KeyRelated
	void InteractKey();
	void CrouchKey();
	void JumpOrEvadeKey();
	void LightAttackKey();
	void HeavyAttackKey();
	void ToggleStanceKey();
	void DefenseKey();
	void ToggleWalkRunKey();
	void SprintKeyPressed();
	void SprintKeyReleased();

	void ToggleTargetKey();
	//==========================================================================================//

	UFUNCTION(BlueprintCallable)
	const FHitResult Trace();

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void OnTargetingBegin(bool Successed);
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void OnTargetingEnd();

	//IGenericTeamAgent Interfaces Related
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override { TeamId = NewTeamId; }
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
	//====================================================================================//

protected :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	FText Name;

	UPROPERTY(BlueprintReadOnly)
	float LookUpInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	bool bGotStartGears;

	UPROPERTY()
	uint32 bInCameraReset : 1;

	UPROPERTY()
	FVector TargetCameraFacingDirection;
};