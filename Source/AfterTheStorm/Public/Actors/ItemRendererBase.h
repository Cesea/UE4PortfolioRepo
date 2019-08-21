// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemRendererBase.generated.h"

UCLASS()
class AFTERTHESTORM_API AItemRendererBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemRendererBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "ItemRender")
	void UpdateMeshToRender(TSubclassOf<class APickupBase> PreviewItemClass);

	UFUNCTION(BlueprintCallable, Category = "ItemRender")
	void ClearPreview();

public:	
public :
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ItemRenderer")
	class USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="ItemRenderer")
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="ItemRenderer")
	class USceneCaptureComponent2D* SceneCaptureComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ItemRenderer")
	class UStaticMeshComponent* BlackBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ItemRenderer")
	class USceneComponent* PreviewItemParent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ItemRenderer")
	class UPointLightComponent* PointLight;

protected :

	AActor* SpawnedPreviewActor;
};
