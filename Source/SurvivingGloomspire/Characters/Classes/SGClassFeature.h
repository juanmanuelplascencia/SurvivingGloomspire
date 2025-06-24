// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGClassFeature.generated.h"

/**
 * Structure representing a class feature
 */
USTRUCT(BlueprintType)
struct FSGClassFeature
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Feature")
    FName FeatureName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Feature")
    FText Description;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Feature")
    int32 LevelUnlocked = 1;
    
    // Optional data specific to this feature
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Feature")
    TMap<FName, FString> FeatureData;
};
