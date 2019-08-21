// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CreationOption.generated.h"

USTRUCT(BlueprintType)
struct FSkeletalMeshCreationData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText OptionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMesh* Mesh;
};

USTRUCT(BlueprintType)
struct FTextureCreationData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText OptionName;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* Texture;
};

/**
 * 
 */
UCLASS()
class AFTERTHESTORM_API USkeletalMeshCreationOption : public UPrimaryDataAsset
{
	GENERATED_BODY()

public :

	UFUNCTION(BlueprintCallable, BlueprintPure)
		const FSkeletalMeshCreationData& GetData(int32 Index) { return Datas[Index]; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSkeletalMeshCreationData> Datas;
};

UCLASS()
class AFTERTHESTORM_API UTextureCreationOption : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const FTextureCreationData& GetData(int32 Index) { return Datas[Index]; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTextureCreationData> Datas;
};