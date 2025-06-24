// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGFeatTypes.h"
#include "SGFeatPrerequisite.generated.h"

/**
 * Structure defining a single prerequisite for a feat
 */
USTRUCT(BlueprintType)
struct FSGFeatPrerequisite
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feat")
    ESGFeatPrerequisiteType PrerequisiteType;
    
    // The value required for the prerequisite
    // Interpretation depends on PrerequisiteType
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feat")
    int32 Value;
    
    // For attribute, skill, or specific feat requirements
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feat")
    FName Subtype;
    
    // Human-readable description of the prerequisite
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feat")
    FText Description;
    
    FSGFeatPrerequisite()
        : PrerequisiteType(ESGFeatPrerequisiteType::None)
        , Value(0)
    {
    }
    
    FSGFeatPrerequisite(ESGFeatPrerequisiteType InType, int32 InValue = 0, const FName& InSubtype = NAME_None, const FText& InDescription = FText::GetEmpty())
        : PrerequisiteType(InType)
        , Value(InValue)
        , Subtype(InSubtype)
        , Description(InDescription)
    {
    }
};
