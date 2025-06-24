// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGClassType.h"
#include "SGClassLevelData.h"
#include "SGCharacterClass.generated.h"

/**
 * Structure containing all data for a character class
 */
USTRUCT(BlueprintType)
struct FSGClassData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Data")
    ESGClassType ClassType = ESGClassType::None;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Data")
    FText DisplayName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Data")
    FText Description;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Data")
    TArray<FSGClassLevelData> LevelData;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Data")
    TArray<FName> ClassSkills;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Data")
    int32 SkillRanksPerLevel = 0;
    
    /**
     * Get the level data for a specific class level (1-based index)
     */
    const FSGClassLevelData* GetLevelData(int32 Level) const
    {
        if (Level > 0 && Level <= LevelData.Num())
        {
            return &LevelData[Level - 1];
        }
        return nullptr;
    }
};

/**
 * Structure representing a character's class level
 */
USTRUCT(BlueprintType)
struct FSGCharacterClassLevel
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Class")
    ESGClassType ClassType = ESGClassType::None;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Class")
    int32 Level = 1;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Class")
    int32 HitPoints = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Class")
    TArray<FName> SelectedFeatures;
};
