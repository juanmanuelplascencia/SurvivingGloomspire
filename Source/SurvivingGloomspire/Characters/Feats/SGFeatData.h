// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGFeatTypes.h"
#include "SGFeatPrerequisite.h"
#include "SGFeatBenefit.h"
#include "SGFeatData.generated.h"

/**
 * Data class that defines the properties of a feat
 */
UCLASS(Blueprintable, BlueprintType)
class SURVIVINGGLOOMSPIRE_API USGFeatData : public UObject
{
    GENERATED_BODY()

public:
    USGFeatData();

    // The type of this feat
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feat")
    ESGFeatType FeatType;
    
    // Display name of the feat
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feat")
    FText DisplayName;
    
    // Detailed description of the feat
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feat")
    FText Description;
    
    // Category of the feat (Combat, Metamagic, Item Creation, etc.)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feat")
    FName Category;
    
    // Whether this feat can be taken multiple times
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feat")
    bool bCanTakeMultipleTimes;
    
    // Stack count if taken multiple times (0 = no stacking)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feat")
    int32 MaxStackCount;
    
    // Prerequisites for taking this feat
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feat")
    TArray<FSGFeatPrerequisite> Prerequisites;
    
    // Benefits provided by this feat
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feat")
    TArray<FSGFeatBenefit> Benefits;
    
    // Whether this feat is a combat feat
    UFUNCTION(BlueprintCallable, Category = "Feat")
    bool IsCombatFeat() const { return Category == FName("Combat"); }
    
    // Whether this feat is a metamagic feat
    UFUNCTION(BlueprintCallable, Category = "Feat")
    bool IsMetamagicFeat() const { return Category == FName("Metamagic"); }
    
    // Whether this feat is an item creation feat
    UFUNCTION(BlueprintCallable, Category = "Feat")
    bool IsItemCreationFeat() const { return Category == FName("ItemCreation"); }
    
    // Get all prerequisites as a formatted text
    UFUNCTION(BlueprintCallable, Category = "Feat")
    FText GetPrerequisitesText() const;
    
    // Check if a character meets all prerequisites for this feat
    UFUNCTION(BlueprintCallable, Category = "Feat")
    bool ArePrerequisitesMet(class ASGCharacterBase* Character) const;
    
    // Apply this feat's benefits to a character
    UFUNCTION(BlueprintCallable, Category = "Feat")
    void ApplyBenefits(class ASGCharacterBase* Character, int32 StackCount = 1) const;
    
    // Remove this feat's benefits from a character
    UFUNCTION(BlueprintCallable, Category = "Feat")
    void RemoveBenefits(class ASGCharacterBase* Character) const;
    
protected:
    // Check if a specific prerequisite is met
    bool CheckPrerequisite(const FSGFeatPrerequisite& Prereq, class ASGCharacterBase* Character) const;
};
