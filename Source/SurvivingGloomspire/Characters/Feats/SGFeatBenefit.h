// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGFeatBenefit.generated.h"

/**
 * Structure defining the benefits provided by a feat
 */
USTRUCT(BlueprintType)
struct FSGFeatBenefit
{
    GENERATED_BODY()

    // Type of benefit this feat provides
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feat")
    FName BenefitType;
    
    // Value associated with the benefit
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feat")
    float Value;
    
    // Additional data for the benefit
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feat")
    FString Data;
    
    // Human-readable description of the benefit
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feat")
    FText Description;
    
    FSGFeatBenefit()
        : Value(0.0f)
    {
    }
    
    FSGFeatBenefit(const FName& InType, float InValue = 0.0f, const FString& InData = TEXT(""), const FText& InDescription = FText::GetEmpty())
        : BenefitType(InType)
        , Value(InValue)
        , Data(InData)
        , Description(InDescription)
    {
    }
};
