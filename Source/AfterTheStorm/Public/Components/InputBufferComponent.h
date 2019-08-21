// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Public/Enums/CharacterEnums.h"
#include "InputBufferComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERTHESTORM_API UInputBufferComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInputBufferComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "InputBuffer")
	void OpenInputBuffer();

	UFUNCTION(BlueprintCallable, Category = "InputBuffer")
	void CloseInputBuffer();

	UFUNCTION(BlueprintCallable, Category = "InputBuffer")
	void ConsumeInputBuffer();

	UFUNCTION(BlueprintCallable, Category = "InputBuffer")
	void UpdateKey(EInputBufferKey NewKey);

	UFUNCTION(BlueprintCallable, Category = "InputBuffer")
	FORCEINLINE EInputBufferKey GetStoredKey() { return StoredKey; }

	DECLARE_EVENT_OneParam(UInputBufferComponent, FInputBufferConsumedEvent, EInputBufferKey)
	DECLARE_EVENT(UInputBufferComponent, FInputBufferOpenEvent)
	DECLARE_EVENT(UInputBufferComponent, FInputBufferCloseEvent)

	FORCEINLINE bool IsOpened() { return bIsOpen; }

public :
	FInputBufferConsumedEvent OnConsumedEvent;
	FInputBufferOpenEvent OnOpenEvent;
	FInputBufferCloseEvent OnCloseEvent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="InputBuffer")
	uint32 bIsOpen : 1;

protected :

	EInputBufferKey StoredKey;
	
};
