// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SGAttributeType.h"
#include "SGAttributeData.h"
#include "SGHitPoints.h"
#include "SGArmorClass.h"
#include "SGSavingThrows.h"
#include "SGClassComponent.h"
#include "SGSkillComponent.h"
#include "SGCharacterBase.generated.h"

// Forward declarations
class USGAttributeSetBase;
class USGAbilitySystemComponent;

/**
 * Base class for all characters in the game.
 * Handles core character functionality including attributes, abilities, and common character features.
 */
UCLASS(Blueprintable, Abstract, meta = (DisplayName = "SG Character Base"))
class SURVIVINGGLOOMSPIRE_API ASGCharacterBase : public ACharacter
{
    GENERATED_BODY()
    
public:
    // ======================================================================
    // Construction & Core Overrides
    // ======================================================================
    
    /** Default constructor */
    ASGCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
    
    //~ Begin AActor Interface
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    //~ End AActor Interface
    
    // ======================================================================
    // Attribute Management - Public Interface
    // ======================================================================
    
    /**
     * Gets the attribute data for the specified attribute type.
     * @param AttributeType - The type of attribute to retrieve
     * @return The attribute data structure
     */
    UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
    FSGAttributeData GetAttribute(ESGAttributeType AttributeType) const;

    /**
     * Sets the base value of the specified attribute and recalculates its modifier.
     * @param AttributeType - The type of attribute to modify
     * @param NewValue - The new base value (will be clamped between 1-30)
     */
    UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
    void SetBaseAttribute(ESGAttributeType AttributeType, int32 NewValue);

    /**
     * Recalculates all attribute modifiers and derived attributes.
     * Call this after making multiple attribute changes for better performance.
     */
    UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
    void CalculateAllModifiers();

    /**
     * Gets the modifier for the specified attribute.
     * @param AttributeType - The type of attribute
     * @return The calculated modifier value
     */
    UFUNCTION(BlueprintCallable, Category = "Character|Attributes")
    int32 GetAttributeModifier(ESGAttributeType AttributeType) const;
    
    /**
     * Gets the current value of an attribute.
     * @param AttributeType - The type of attribute
     * @return The current base value
     */
    UFUNCTION(BlueprintPure, Category = "Character|Attributes")
    int32 GetAttributeValue(ESGAttributeType AttributeType) const;
    
    /**
     * Gets the character's hit points
     * @return Reference to the hit points structure
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character|Attributes")
    const FSGHitPoints& GetHitPoints() const { return HitPoints; }
    
    /**
     * Gets the character's armor class
     * @return Reference to the armor class structure
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character|Attributes")
    const FSGArmorClass& GetArmorClass() const { return ArmorClass; }
    
    /**
     * Gets the character's saving throws
     * @return Reference to the saving throws structure
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character|Attributes")
    const FSGSavingThrows& GetSavingThrows() const { return SavingThrows; }
    
    /**
     * Gets the base values used for derived attribute calculations
     * @return Base hit points from class and level
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character|Attributes")
    int32 GetBaseHitPoints() const { return BaseHitPoints; }

    /**
     * Helper function to get attribute display name as string.
     * @param AttributeType - The type of attribute
     * @return Display name as FString
     */
    UFUNCTION(BlueprintPure, Category = "Character|Attributes")
    static FString GetAttributeName(ESGAttributeType AttributeType);

    // ======================================================================
    // Derived Attributes & Combat - Public Interface
    // ======================================================================
    
    /**
     * Calculates all derived attributes based on current base attributes and base values
     */
    UFUNCTION(BlueprintCallable, Category = "Character|Combat")
    void CalculateDerivedAttributes();
    
    /**
     * Applies damage to the character, reducing hit points
     * @param Amount Amount of damage to apply
     * @return True if the character was brought to 0 or fewer hit points
     */
    UFUNCTION(BlueprintCallable, Category = "Character|Combat")
    bool ApplyDamage(int32 Amount);
    
    /**
     * Heals the character, restoring hit points up to maximum
     * @param Amount Amount of healing to apply
     * @return Actual amount of healing applied
     */
    UFUNCTION(BlueprintCallable, Category = "Character|Combat")
    int32 ApplyHealing(int32 Amount);

    // ======================================================================
    // Class & Progression - Public Interface
    // ======================================================================
    
    /**
     * Gets the character's class component
     * @return The class component that handles class progression
     */
    UFUNCTION(BlueprintCallable, Category = "Character|Progression")
    USGClassComponent* GetClassComponent() const { return ClassComponent; }
    
    // ======================================================================
    // Skills - Public Interface
    // ======================================================================
    
    /**
     * Gets the character's skill component
     * @return The skill component that handles skill ranks and checks
     */
    UFUNCTION(BlueprintCallable, Category = "Character|Skills")
    USGSkillComponent* GetSkillComponent() const { return SkillComponent; }
    
    // ======================================================================
    // Debug & Development - Public Interface
    // ======================================================================
    
    /**
     * Logs the current state of all attributes (base and derived)
     * @param bForce If true, will log even if debug logging is disabled
     */
    UFUNCTION(BlueprintCallable, Category = "Debug")
    virtual void DebugLogState(bool bForce = false) const;

protected:
    // ======================================================================
    // Core Character Properties
    // ======================================================================
    
    /** Map of all character attributes */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Attributes")
    TMap<ESGAttributeType, FSGAttributeData> Attributes;

    /** Character's hit points */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Attributes")
    FSGHitPoints HitPoints;
    
    /** Character's armor class */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Attributes")
    FSGArmorClass ArmorClass;
    
    /** Character's saving throws */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Attributes")
    FSGSavingThrows SavingThrows;
    
    /** Base hit points from class and level */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Attributes")
    int32 BaseHitPoints = 10;

    // ======================================================================
    // Protected Methods
    // ======================================================================
    
    /** Initializes default attribute values */
    virtual void InitializeDefaultAttributes();
    
    /** Called when an attribute changes value */
    virtual void OnAttributeChanged(ESGAttributeType AttributeType);
    
private:
    // ======================================================================
    // Components
    // ======================================================================
    
    /** Handles character class progression, levels, and experience */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USGClassComponent> ClassComponent;
    
    /** Handles character skills and skill checks */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USGSkillComponent> SkillComponent;
    
    // ======================================================================
    // Private Properties
    // ======================================================================
    
    /** Controls whether debug logging is enabled for this character */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = (AllowPrivateAccess = "true"))
    bool bEnableDebugLogging = true;
};
