// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SGCharacterClass.h"
#include "SGCharacterClassData.generated.h"

/**
 * Data asset that defines the properties and progression of a character class
 */
UCLASS(BlueprintType)
class SURVIVINGGLOOMSPIRE_API USGCharacterClassData : public UPrimaryDataAsset
{
    GENERATED_BODY()
    
public:
    /** Type of this class */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class Data")
    ESGClassType ClassType;
    
    /** Display name of the class */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class Data")
    FText DisplayName;
    
    /** Description of the class */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class Data", meta = (MultiLine = true))
    FText Description;
    
    /** Hit die size (e.g., 10 for d10) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class Data")
    int32 HitDie;
    
    /** Skill ranks per level */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class Data")
    int32 SkillRanksPerLevel;
    
    /** Class skills */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class Data")
    TArray<FName> ClassSkills;
    
    /** Class progression data for each level */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class Data")
    TArray<FSGClassLevelData> LevelData;
    
    /**
     * Get the level data for a specific level (1-based index)
     * @param Level Level to get data for (1-20)
     * @return Reference to level data, or default if invalid level
     */
    UFUNCTION(BlueprintCallable, Category = "Class Data")
    const FSGClassLevelData& GetLevelData(int32 Level) const;
    
    //~ Begin UPrimaryDataAsset Interface
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
    //~ End UPrimaryDataAsset Interface
};
