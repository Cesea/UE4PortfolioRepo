
#include "Public/Actors/Players/PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Engine.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Public/ASGameInstance.h"
#include "Public/Utils/ASSaveGame.h"

#include "Public/CharacterCreation/CreationOption.h"

#include "Public/Components/InteractionComponent.h"
#include "Public/Components/InputBufferComponent.h"
#include "Public/Components/StateMachineComponent.h"
#include "Public/Components/TargetingComponent.h"
#include "Public/Components/EquipmentComponent.h"
#include "Public/Components/ExpComponent.h"
#include "Public/Components/StatComponent.h"
#include "Public/Components/RotationControlComponent.h"
#include "Public/Components/CustomizingComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"	

#include "Public/Interfaces/Interactable.h"
#include "Public/Interfaces/Attackable.h"

#include "DrawDebugHelpers.h"

#include "Public/Actors/Players/MyPlayerController.h"
#include "Public/Components/TargetingComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractionSphereParentComponent = CreateDefaultSubobject<USceneComponent>("InteractionSphereParent");
	InteractionSphereParentComponent->SetupAttachment(RootComponent);

	InteractionSphere->SetupAttachment(InteractionSphereParentComponent);

	//Create SpringArm Component 
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	//And Attach to root
	SpringArmComponent->SetupAttachment(RootComponent);
	//Make SpringArm's rotation same as Control rotation
	SpringArmComponent->bUsePawnControlRotation = true;

	//Create Camera Component 
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	//And Attach to SpringArm
	CameraComponent->SetupAttachment(SpringArmComponent);

	//Setup Targeting Component	
	TargetingComponent = CreateDefaultSubobject<UTargetingComponent>("TargetingComponent");

	//Setup Exp Component
	ExpComponent = CreateDefaultSubobject<UExpComponent>("ExpComponent");

	//Setup RotationControl Component
	RotationControlComponent = CreateDefaultSubobject<URotationControlComponent>("RotationControlComponent");

	//Setup Customizing Component
	CustomizingComponent = CreateDefaultSubobject<UCustomizingComponent>("CustomizingComponent");

	//Setup Status
	StanceState = EStanceState::ESS_Unarmed;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	LoadCharacter();

	SetGenericTeamId(TeamId);
	
	UWorld* World = GetWorld();
	UGameplayStatics::GetPlayerCameraManager(World, 0)->ViewPitchMin = MinPitchValue;
	UGameplayStatics::GetPlayerCameraManager(World, 0)->ViewPitchMax = MaxPitchValue;

	InputBufferComponent->OnConsumedEvent.AddUObject(this, &APlayerCharacter::OnInputBufferConsumed);
	InputBufferComponent->OnCloseEvent.AddUObject(this, &APlayerCharacter::OnInputBufferClose);

	ExpComponent->OnLevelUp.AddDynamic(this, &APlayerCharacter::OnLevelUp);

	AddTickPrerequisiteComponent(RotationControlComponent);

	InventoryComponent->EquipEquipments();
}

void APlayerCharacter::SaveCharacter(UASSaveGame* SaveGame)
{
	if (SaveGame != nullptr)
	{
		SaveGame->PlayerSaveData.Transform = GetActorTransform();

		SaveGame->PlayerSaveData.InventorySave.Gold = InventoryComponent->GetGold();
		SaveGame->PlayerSaveData.InventorySave.InventoryItems = InventoryComponent->GetInventories();

		SaveGame->PlayerSaveData.StatSave.CurrentHealth = HealthStatComponent->GetCurrentValue();
		SaveGame->PlayerSaveData.StatSave.MaxHealth = HealthStatComponent->GetMaxValue();
		SaveGame->PlayerSaveData.StatSave.CurrentStamina = StaminaStatComponent->GetCurrentValue();
		SaveGame->PlayerSaveData.StatSave.MaxStamina = StaminaStatComponent->GetMaxValue();

		SaveGame->PlayerExp = ExpComponent->GetCurrentExp();
		SaveGame->PlayerLevel = ExpComponent->GetLevel();
		SaveGame->PlayerLevelUpExp = ExpComponent->GetLevelUpExp();

		SaveGame->bGotStartGears = this->bGotStartGears;
	}
}

void APlayerCharacter::LoadCharacter()
{
	UASGameInstance* GameInstance = (UASGameInstance*)UGameplayStatics::GetGameInstance(GetWorld());
	if (UGameplayStatics::DoesSaveGameExist(GameInstance->GetCurrentSlotName(), 0))
	{
		UASSaveGame* LoadedGame = (UASSaveGame*)UGameplayStatics::LoadGameFromSlot(GameInstance->GetCurrentSlotName(), 0);
		if (LoadedGame != nullptr)
		{
			Name = FText::FromString(GameInstance->GetCurrentSlotName());

			CustomizingComponent->ApplyCustomizing(LoadedGame->PlayerCustomizingInfo);

			const FCharacterStatSaveData& StatSaveRef = LoadedGame->PlayerSaveData.StatSave;


			bGotStartGears = LoadedGame->bGotStartGears;

			FString CurrentMapName = GetWorld()->GetMapName();
			if (CurrentMapName.Contains("BossMap") && 
				GameInstance->PlayerInventory.Num() > 0)
			{
				InventoryComponent->SetInventory(GameInstance->PlayerInventory);
				InventoryComponent->SetGold(GameInstance->PlayerGold);

				HealthStatComponent->SetCurrentValue(GameInstance->PlayerCurrentHealth);
				HealthStatComponent->SetMaxValue(GameInstance->PlayerMaxHealth);
				StaminaStatComponent->SetCurrentValue(GameInstance->PlayerCurrentStamina);
				StaminaStatComponent->SetMaxValue(GameInstance->PlayerMaxStamina);

				ExpComponent->SetLevel(GameInstance->PlayerLevel);
				ExpComponent->SetLevelUpExp(GameInstance->PlayerLevelUpExp);
				ExpComponent->SetCurrentExp(GameInstance->PlayerCurrentExp);
			}
			else
			{
				if (!LoadedGame->bIsFirstSave)
				{
					InventoryComponent->SetInventory(LoadedGame->PlayerSaveData.InventorySave.InventoryItems);
					InventoryComponent->SetGold(LoadedGame->PlayerSaveData.InventorySave.Gold);

					HealthStatComponent->SetCurrentValue(StatSaveRef.CurrentHealth);
					HealthStatComponent->SetMaxValue(StatSaveRef.MaxHealth);
					StaminaStatComponent->SetCurrentValue(StatSaveRef.CurrentStamina);
					StaminaStatComponent->SetMaxValue(StatSaveRef.MaxStamina);

					ExpComponent->SetLevel(LoadedGame->PlayerLevel);
					ExpComponent->SetLevelUpExp(LoadedGame->PlayerLevelUpExp);
					ExpComponent->SetCurrentExp(LoadedGame->PlayerExp);
				}
			}
			ExpComponent->OnExpUpdated.Broadcast(ExpComponent->GetCurrentExp(), ExpComponent->GetLevelUpExp());
			ExpComponent->OnLevelUp.Broadcast(ExpComponent->GetLevel(), false);


			if (!CurrentMapName.Contains("BossMap"))
			{
				if (!LoadedGame->bIsFirstSave)
				{
					SetActorTransform(LoadedGame->PlayerSaveData.Transform, false, nullptr, ETeleportType::TeleportPhysics);
				}
			}
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InteractionSphereParentComponent->SetWorldRotation(FRotator(0, GetControlRotation().Yaw, 0));

	if (bIsWeaponToggling)
	{
		FVector2D PlayerInputVector(ForwardAxisValue, RightAxisValue);
		if (bWasMovingInToggle)
		{
			if (PlayerInputVector.IsNearlyZero())
			{
				ResetRootmotion();
			}
		}
		else
		{
			if (!PlayerInputVector.IsNearlyZero())
			{
				IgnoreRootmotion();
			}
		}
	}

	if (bInCameraReset)
	{
		FRotator ControlRotation = GetControlRotation();

		FRotator PitchAdjustedActorRotation = GetActorRotation();
		PitchAdjustedActorRotation.Pitch = ControlRotation.Pitch;

		FRotator InterpedRotation = UKismetMathLibrary::RInterpTo(ControlRotation, TargetCameraFacingDirection.Rotation(), DeltaTime, 28.0f);
		GetController()->SetControlRotation(InterpedRotation);

		if (UKismetMathLibrary::EqualEqual_RotatorRotator(InterpedRotation, TargetCameraFacingDirection.Rotation(), 0.2f))
		{
			bInCameraReset = false;
		}
	}

	if (TargetingComponent->IsTargeted())
	{
		TargetCameraFacingDirection = (TargetingComponent->GetTargetedActor()->GetActorLocation() - CameraComponent->GetComponentLocation()).GetSafeNormal();

		FRotator ControlRotation = GetControlRotation();
		FRotator TargetRotation = TargetCameraFacingDirection.Rotation();

		FRotator InterpedRotation = UKismetMathLibrary::RInterpTo(ControlRotation, TargetRotation, DeltaTime, 28.0f);
		GetController()->SetControlRotation(InterpedRotation);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp);

	PlayerInputComponent->BindAction("JumpOrEvade", EInputEvent::IE_Pressed, this, &APlayerCharacter::JumpOrEvadeKey);
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &APlayerCharacter::InteractKey);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &APlayerCharacter::CrouchKey);
	PlayerInputComponent->BindAction("LightAttack", EInputEvent::IE_Pressed, this, &APlayerCharacter::LightAttackKey);
	PlayerInputComponent->BindAction("HeavyAttack", EInputEvent::IE_Pressed, this, &APlayerCharacter::HeavyAttackKey);
	PlayerInputComponent->BindAction("Defense", EInputEvent::IE_Pressed, this, &APlayerCharacter::DefenseKey);
	PlayerInputComponent->BindAction("ToggleStance", EInputEvent::IE_Pressed, this, &APlayerCharacter::ToggleStanceKey);

	PlayerInputComponent->BindAction("ToggleWalkRun", EInputEvent::IE_Pressed, this, &APlayerCharacter::ToggleWalkRunKey);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &APlayerCharacter::SprintKeyPressed);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &APlayerCharacter::SprintKeyReleased);

	PlayerInputComponent->BindAction("ToggleTarget", EInputEvent::IE_Pressed, this, &APlayerCharacter::ToggleTargetKey);
}


void APlayerCharacter::OnLevelUp_Implementation(int32 NewLevel, bool bIsInitial)
{
	if (!bIsInitial)
	{
		HealthStatComponent->SetMaxValue((int32)(HealthStatComponent->GetMaxValue() * 1.5f));
		HealthStatComponent->SetCurrentValue(HealthStatComponent->GetMaxValue());

		StaminaStatComponent->SetMaxValue((int32)(StaminaStatComponent->GetMaxValue() * 1.5f));
		StaminaStatComponent->SetCurrentValue(StaminaStatComponent->GetMaxValue());
	}
}

void APlayerCharacter::OnTargetingSuccess(AActor* TargetedActor)
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

void APlayerCharacter::OnTargetingFail()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void APlayerCharacter::MoveForward(float Value)
{
	ForwardAxisValue = Value;
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
	AddMovementInput(ForwardVector, Value);
}

void APlayerCharacter::MoveRight(float Value)
{
	RightAxisValue = Value;
	FVector RightVector = UKismetMathLibrary::GetRightVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
	AddMovementInput(RightVector, Value);
}

void APlayerCharacter::Turn(float Value)
{
	if (!TargetingComponent->IsTargeted())
	{
		AddControllerYawInput(Value);
	}
}

void APlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void APlayerCharacter::InteractKey()
{
	InputBufferComponent->UpdateKey(EInputBufferKey::EIBK_Interact);
}

void APlayerCharacter::CrouchKey()
{
	InputBufferComponent->UpdateKey(EInputBufferKey::EIBK_Crouch);
}

void APlayerCharacter::JumpOrEvadeKey()
{
	InputBufferComponent->UpdateKey(EInputBufferKey::EIBK_JumpOrEvade);
}

void APlayerCharacter::LightAttackKey()
{
	InputBufferComponent->UpdateKey(EInputBufferKey::EIBK_LightAttack);
}

void APlayerCharacter::HeavyAttackKey()
{
	InputBufferComponent->UpdateKey(EInputBufferKey::EIBK_HeavyAttack);
}

void APlayerCharacter::ToggleStanceKey()
{
	InputBufferComponent->UpdateKey(EInputBufferKey::EIBK_ToggleStance);
}

void APlayerCharacter::DefenseKey()
{
	InputBufferComponent->UpdateKey(EInputBufferKey::EIBK_Defense);
}

void APlayerCharacter::ToggleWalkRunKey()
{
	InputBufferComponent->UpdateKey(EInputBufferKey::EIBK_ToggleWalkRun);
}

void APlayerCharacter::SprintKeyPressed()
{
	InputBufferComponent->UpdateKey(EInputBufferKey::EIBK_StartSprint);
}

void APlayerCharacter::SprintKeyReleased()
{
	InputBufferComponent->UpdateKey(EInputBufferKey::EIBK_StopSprint);
}

void APlayerCharacter::ToggleTargetKey()
{
	if (TargetingComponent != nullptr)
	{
		if (TargetingComponent->IsTargeted())
		{
			TargetingComponent->UnlockTarget();
		}
		else
		{
			TargetingComponent->LockTarget();
		}
	}
}

const FHitResult APlayerCharacter::Trace()
{
	FHitResult HitResult;

	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		if (APlayerController * PlayerController = Cast<APlayerController>(GetController()))
		{
			FVector Start;
			FRotator ViewRotation;
			FVector End;
			PlayerController->GetPlayerViewPoint(Start, ViewRotation);
			End = Start + ViewRotation.Vector() * TraceDistance;

			World->LineTraceSingleByChannel(HitResult, Start, End,
				ECollisionChannel::ECC_Visibility,
				FCollisionQueryParams::DefaultQueryParam);
		}
	}

	return HitResult;
}

void APlayerCharacter::OnTargetingBegin(bool Successed)
{
	if (Successed)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;

		TargetCameraFacingDirection = (TargetingComponent->GetTargetedActor()->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	}
	else
	{
		bInCameraReset = true;
		TargetCameraFacingDirection = GetActorForwardVector();
	}
}

void APlayerCharacter::OnTargetingEnd()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

}

void APlayerCharacter::ResetStatus()
{
	Super::ResetStatus();
	if (TargetingComponent->IsTargeted())
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
}
