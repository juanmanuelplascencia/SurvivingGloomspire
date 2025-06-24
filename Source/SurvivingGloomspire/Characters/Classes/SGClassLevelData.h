// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGClassFeature.h"
#include "SGClassLevelData.generated.h"

/**
 * Structure containing data for a single class level
 */
USTRUCT(BlueprintType)
struct FSGClassLevelData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Level")
    int32 BaseAttackBonus = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Level")
    int32 FortitudeSave = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Level")
    int32 ReflexSave = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Level")
    int32 WillSave = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Level")
    int32 SkillRanks = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Level")
    TArray<FSGClassFeature> FeaturesGranted;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Level")
    int32 HitDie = 0;  // Hit die size (e.g., 10 for d10)
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Level")
    int32 SpellsPerDayLevel = 0;  // 0 if no spellcasting at this level
};
