// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SGFeatTypes.h"
#include "SGFeatData.h"
#include "SGFeatComponent.generated.h"

class USGFeatData;
class ASGCharacterBase;

/** Structure to track a feat and its current stack count */
USTRUCT(BlueprintType)
struct FSGFeatInstance
{
    GENERATED_BODY()

    // The type of feat
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Feat")
    ESGFeatType FeatType;
    
    // The data for this feat
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Feat")
    TObjectPtr<USGFeatData> FeatData;
    
    // Current stack count (for feats that can be taken multiple times)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Feat")
    int32 StackCount;
    
    FSGFeatInstance()
        : FeatType(ESGFeatType::MAX)
        , FeatData(nullptr)
        , StackCount(0)
    {
    }
    
    FSGFeatInstance(ESGFeatType InFeatType, USGFeatData* InFeatData, int32 InStackCount = 1)
        : FeatType(InFeatType)
        , FeatData(InFeatData)
        , StackCount(InStackCount)
    {
    }
    
    // Check if this feat instance is valid
    bool IsValid() const { return FeatType != ESGFeatType::MAX && FeatData != nullptr; }
};

/**
 * Component that manages a character's feats
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVINGGLOOMSPIRE_API USGFeatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    USGFeatComponent();

    //~ Begin UActorComponent Interface
    virtual void BeginPlay() override;
    //~ End UActorComponent Interface

    /**
     * Initialize the feat component with the owner character
     */
    void Initialize(ASGCharacterBase* InOwnerCharacter);

    /**
     * Check if the character has a specific feat
     * @param FeatType The type of feat to check for
     * @return True if the character has the feat, false otherwise
     */
    UFUNCTION(BlueprintCallable, Category = "Feats")
    bool HasFeat(ESGFeatType FeatType) const;
    
    /**
     * Get the stack count for a feat
     * @param FeatType The type of feat to check
     * @return The current stack count (0 if the character doesn't have the feat)
     */
    UFUNCTION(BlueprintCallable, Category = "Feats")
    int32 GetFeatStackCount(ESGFeatType FeatType) const;
    
    /**
     * Add a feat to the character
     * @param FeatType The type of feat to add
     * @param bCheckPrerequisites Whether to check prerequisites before adding
     * @return True if the feat was added successfully, false otherwise
     */
    UFUNCTION(BlueprintCallable, Category = "Feats")
    bool AddFeat(ESGFeatType FeatType, bool bCheckPrerequisites = true);
    
    /**
     * Remove a feat from the character
     * @param FeatType The type of feat to remove
     * @return True if the feat was removed, false if the character didn't have it
     */
    UFUNCTION(BlueprintCallable, Category = "Feats")
    bool RemoveFeat(ESGFeatType FeatType);
    
    /**
     * Get all feats the character has
     * @return Array of feat instances
     */
    UFUNCTION(BlueprintCallable, Category = "Feats")
    const TArray<FSGFeatInstance>& GetFeats() const { return Feats; }
    
    /**
     * Check if the character meets all prerequisites for a feat
     * @param FeatType The type of feat to check
     * @return True if all prerequisites are met, false otherwise
     */
    UFUNCTION(BlueprintCallable, Category = "Feats")
    bool MeetsPrerequisites(ESGFeatType FeatType) const;
    
    /**
     * Get the number of feats the character has of a specific category
     * @param Category The category to count feats for
     * @return The number of feats in the specified category
     */
    UFUNCTION(BlueprintCallable, Category = "Feats")
    int32 GetFeatCountByCategory(FName Category) const;
    
    /**
     * Get all feats of a specific category
     * @param Category The category to get feats for
     * @param OutFeats Output array of feats in the category
     */
    UFUNCTION(BlueprintCallable, Category = "Feats")
    void GetFeatsByCategory(FName Category, TArray<FSGFeatInstance>& OutFeats) const;

protected:
    /** The character that owns this component */
    UPROPERTY()
    TWeakObjectPtr<ASGCharacterBase> OwnerCharacter;
    
    /** All feats the character has */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Feats", meta = (AllowPrivateAccess = "true"))
    TArray<FSGFeatInstance> Feats;
    
    /**
     * Get the feat data for a feat type
     * @param FeatType The type of feat to get data for
     * @return The feat data, or nullptr if not found
     */
    USGFeatData* GetFeatData(ESGFeatType FeatType) const;
    
    /**
     * Find an existing feat instance
     * @param FeatType The type of feat to find
     * @return Pointer to the feat instance, or nullptr if not found
     */
    FSGFeatInstance* FindFeatInstance(ESGFeatType FeatType);
    
    /**
     * Find an existing feat instance (const version)
     * @param FeatType The type of feat to find
     * @return Pointer to the feat instance, or nullptr if not found
     */
    const FSGFeatInstance* FindFeatInstance(ESGFeatType FeatType) const;
};
