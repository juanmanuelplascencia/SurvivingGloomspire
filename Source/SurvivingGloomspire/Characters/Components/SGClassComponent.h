// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SGCharacterClass.h"
#include "SGClassComponent.generated.h"

// Delegate for when a character gains a level
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCharacterLevelUp, AActor*, Character, ESGClassType, ClassType, int32, NewLevel);

// Delegate for when a character gains experience
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnExperienceGained, AActor*, Character, int32, ExperienceGained, int32, NewTotal);

/**
 * Component that handles character class progression, levels, and experience
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVINGGLOOMSPIRE_API USGClassComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    USGClassComponent();

    /**
     * Add experience points to the character
     * @param Amount Amount of XP to add
     * @return True if the character leveled up
     */
    UFUNCTION(BlueprintCallable, Category = "Character|Progression")
    bool AddExperience(int32 Amount);
    
    /**
     * Get the current experience points
     */
    UFUNCTION(BlueprintPure, Category = "Character|Progression")
    int32 GetCurrentXP() const { return CurrentXP; }
    
    /**
     * Get the experience needed for the next level
     */
    UFUNCTION(BlueprintPure, Category = "Character|Progression")
    int32 GetXPForNextLevel() const;
    
    /**
     * Get the character's current level in their primary class
     */
    UFUNCTION(BlueprintPure, Category = "Character|Progression")
    int32 GetCharacterLevel() const;
    
    /**
     * Get the character's class levels
     */
    UFUNCTION(BlueprintPure, Category = "Character|Progression")
    const TArray<FSGCharacterClassLevel>& GetClassLevels() const { return ClassLevels; }
    
    /**
     * Add a level in the specified class
     * @param ClassType The class to level up in
     * @param bAllowMulticlass Whether to allow multiclassing
     * @return True if the level was added successfully
     */
    UFUNCTION(BlueprintCallable, Category = "Character|Progression")
    bool AddClassLevel(ESGClassType ClassType, bool bAllowMulticlass = false);
    
    /**
     * Get the total number of levels the character has
     */
    UFUNCTION(BlueprintPure, Category = "Character|Progression")
    int32 GetTotalLevels() const;

    /**
     * Get the character's base attack bonus from all classes
     */
    UFUNCTION(BlueprintPure, Category = "Character|Progression")
    int32 GetBaseAttackBonus() const;
    
    /**
     * Called when the character gains a level
     */
    UPROPERTY(BlueprintAssignable, Category = "Character|Progression")
    FOnCharacterLevelUp OnLevelUp;
    
    /**
     * Called when the character gains experience
     */
    UPROPERTY(BlueprintAssignable, Category = "Character|Progression")
    FOnExperienceGained OnExperienceGained;

protected:
    virtual void BeginPlay() override;
    
    /**
     * Calculate experience needed for a given level
     */
    UFUNCTION(BlueprintCallable, Category = "Character|Progression")
    static int32 CalculateXPForLevel(int32 Level);
    
    /**
     * Apply features from the character's class levels
     */
    void ApplyClassFeatures();
    
    /**
     * Apply a single class feature
     */
    void ApplyClassFeature(const FSGClassFeature& Feature);

private:
    /** Current experience points */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Progression", meta = (AllowPrivateAccess = "true"))
    int32 CurrentXP;
    
    /** Character's class levels */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Progression", meta = (AllowPrivateAccess = "true"))
    TArray<FSGCharacterClassLevel> ClassLevels;
    
    /** Cached pointer to the owning character */
    UPROPERTY()
    TWeakObjectPtr<AActor> OwnerCharacter;
};
