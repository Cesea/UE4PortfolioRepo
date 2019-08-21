// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
 #include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomizingComponent.generated.h"

USTRUCT(BlueprintType)
struct AFTERTHESTORM_API FCharacterCustomizingInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HeadIndex {0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HairIndex {-1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FaceHairIndex {-1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WarpaintIndex {-1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor HairColor { FColor::Black };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor FaceHairColor { FColor::Black };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor WarpaintTint {FColor::Black};
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERTHESTORM_API UCustomizingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCustomizingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public :
	UFUNCTION(BlueprintCallable)
	void ApplyCustomizing(const FCharacterCustomizingInfo& NewInfo);

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Customizing")
	FCharacterCustomizingInfo CustomizingInfo;

protected :
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Customozing")
	bool bCanApplyCustomizing{true};
};
