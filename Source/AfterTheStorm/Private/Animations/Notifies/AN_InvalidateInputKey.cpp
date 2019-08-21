// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Animations/Notifies/AN_InvalidateInputKey.h"

#include "Components/SkeletalMeshComponent.h"

#include "Public/Components/InputBufferComponent.h"


void UAN_InvalidateInputKey::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Owner != nullptr)
	{
		UInputBufferComponent* InputBuffer = Owner->FindComponentByClass<UInputBufferComponent>();
		if (InputBuffer != nullptr)
		{
			InputBuffer->UpdateKey(EInputBufferKey::EIBK_Invalid);
		}
	}
}
