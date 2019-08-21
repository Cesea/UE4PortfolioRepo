// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/CustomizingComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Components/SkeletalMeshComponent.h"

#include "Materials/MaterialInstanceDynamic.h"

#include "Public/ASGameInstance.h"

#include "Public/Actors/MultiPartCharacterBase.h"

#include "Public/CharacterCreation/CreationOption.h"

// Sets default values for this component's properties
UCustomizingComponent::UCustomizingComponent()
{
}


// Called when the game starts
void UCustomizingComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
}

void UCustomizingComponent::ApplyCustomizing(const FCharacterCustomizingInfo& NewInfo)
{
	CustomizingInfo = NewInfo;

	if (bCanApplyCustomizing)
	{

		UASGameInstance* GameInstance = Cast<UASGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (AMultiPartCharacterBase * Character = Cast<AMultiPartCharacterBase>(GetOwner()))
		{
			//First Setup Head
			Character->GetMesh()->SetSkeletalMesh(GameInstance->GetHeadOptions()->Datas[CustomizingInfo.HeadIndex].Mesh);

			//Second Setup Hair
			if (CustomizingInfo.HairIndex >= 0)
			{
				USkeletalMeshComponent* HairMeshComponent = NewObject<USkeletalMeshComponent>(Character, "HairMesh");
				HairMeshComponent->RegisterComponentWithWorld(GetWorld());
				HairMeshComponent->SetSkeletalMesh(GameInstance->GetHairOptions()->Datas[CustomizingInfo.HairIndex].Mesh);

				UMaterialInstanceDynamic* InstanceMaterial = HairMeshComponent->CreateDynamicMaterialInstance(0);
				InstanceMaterial->SetVectorParameterValue("RootColor", CustomizingInfo.HairColor);
				InstanceMaterial->SetVectorParameterValue("TipColor", CustomizingInfo.HairColor);
				HairMeshComponent->SetMaterial(0, InstanceMaterial);

				HairMeshComponent->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
				HairMeshComponent->SetMasterPoseComponent(Character->GetMesh());
			}
			//Third Setup FaceHair
			if (CustomizingInfo.FaceHairIndex >= 0)
			{
				USkeletalMeshComponent* FaceHairMeshComponent = NewObject<USkeletalMeshComponent>(Character, "FaceHairMesh");
				FaceHairMeshComponent->RegisterComponentWithWorld(GetWorld());
				FaceHairMeshComponent->SetSkeletalMesh(GameInstance->GetFaceHairOptions()->Datas[CustomizingInfo.FaceHairIndex].Mesh);

				UMaterialInstanceDynamic* InstanceMaterial = FaceHairMeshComponent->CreateDynamicMaterialInstance(0);
				InstanceMaterial->SetVectorParameterValue("RootColor", CustomizingInfo.HairColor);
				InstanceMaterial->SetVectorParameterValue("TipColor", CustomizingInfo.HairColor);
				FaceHairMeshComponent->SetMaterial(0, InstanceMaterial);

				FaceHairMeshComponent->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
				FaceHairMeshComponent->SetMasterPoseComponent(Character->GetMesh());
			}
			//Warpaint Setup
			if (CustomizingInfo.WarpaintIndex >= 0)
			{
				UMaterialInstanceDynamic* InstanceMaterial = Character->GetMesh()->CreateDynamicMaterialInstance(0);
				InstanceMaterial->SetTextureParameterValue("WarPaintTex", GameInstance->GetWarpaintOptions()->GetData(CustomizingInfo.WarpaintIndex).Texture);
				InstanceMaterial->SetVectorParameterValue("WarPaint", CustomizingInfo.WarpaintTint);
				Character->GetMesh()->SetMaterial(0, InstanceMaterial);
			}
		}
	}
}
