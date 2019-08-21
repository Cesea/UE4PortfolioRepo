// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Actors/ItemRendererBase.h"

#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/StaticMeshComponent.h"

#include "Engine/World.h"

#include "Public/Actors/PickupBase.h"

// Sets default values
AItemRendererBase::AItemRendererBase()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("Scene");
	RootComponent = SceneComponent;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(RootComponent);

	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>("SceneCapture");
	SceneCaptureComponent->SetupAttachment(CameraComponent);

	BlackBox = CreateDefaultSubobject<UStaticMeshComponent>("BlackBox");
	BlackBox->SetupAttachment(RootComponent);

	PreviewItemParent = CreateDefaultSubobject<USceneComponent>("PreviewParent");
	PreviewItemParent->SetupAttachment(BlackBox);

	PointLight = CreateDefaultSubobject<UPointLightComponent>("PointLight");
	PointLight->SetupAttachment(BlackBox);
}

// Called when the game starts or when spawned
void AItemRendererBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemRendererBase::UpdateMeshToRender(TSubclassOf<class APickupBase> PreviewItemClass)
{
	if (SpawnedPreviewActor != nullptr)
	{
		SpawnedPreviewActor->Destroy();
	}

	if (UWorld * World = GetWorld())
	{
		FTransform SpawnTransform = GetActorTransform();
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		APickupBase* SpawnedPickup = World->SpawnActor<APickupBase>(PreviewItemClass, SpawnTransform, SpawnParameters);

		SpawnedPreviewActor = SpawnedPickup;

		SpawnedPickup->DisableCollisions();
		SpawnedPickup->AttachToComponent(PreviewItemParent, FAttachmentTransformRules::SnapToTargetIncludingScale);

		SceneCaptureComponent->CaptureScene();
	}
}

void AItemRendererBase::ClearPreview()
{
	if (SpawnedPreviewActor != nullptr)
	{
		SpawnedPreviewActor->Destroy();
		SceneCaptureComponent->CaptureScene();
	}
}
