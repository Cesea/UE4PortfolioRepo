// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Public/Actors/InteractableBase.h"

#include "Engine/DataTable.h"
#include "PaperSprite.h"

#include "Public/Enums/ItemEnums.h"
#include "PickupBase.generated.h"

USTRUCT(BlueprintType)
struct AFTERTHESTORM_API FItemData : public FTableRowBase
{
	GENERATED_BODY()
public:
	FItemData() 
		:ItemID("1"), Name(FText::FromString("NotSet")), Description(FText::FromString("NotSet")), bIsStackable(true)
	{
	}
	FItemData(FName ItemID)
		:ItemID(ItemID), Name(FText::FromString("NotSet")), Description(FText::FromString("NotSet")), bIsStackable(true)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSprite* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bIsStackable : 1;

	bool operator== (const FItemData& Other) const
	{
		if (ItemID == Other.ItemID)
			return true;

		return false;
	}
};

/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API APickupBase : public AInteractableBase
{
	GENERATED_BODY()

protected:
	APickupBase();
	virtual void BeginPlay() override;
public :
	virtual void Interact_Implementation(AActor* Actor) override;
	virtual void ShowFeedbackWidget_Implementation(AActor* Actor) override;
	virtual void HideFeedbackWidget_Implementation(AActor* Actor) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pickup")
	class USkeletalMeshComponent* Mesh;

public:
	//UFUNCTION(BlueprintCallable, Category = "Pickup")
	//void Drop(AActor* Actor);

	FORCEINLINE FName GetID() { return ID; }

protected :
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	FName ID;

	FItemData* ItemData;
};
