// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JigsawItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class JIGSAWINVEN_API UJigsawItem : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint ItemSize = FIntPoint(1, 1);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> ItemImage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStackSize = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentStackSize = 1;

};
