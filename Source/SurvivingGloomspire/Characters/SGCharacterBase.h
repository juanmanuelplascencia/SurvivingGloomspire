// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SGAttributeTypes.h"
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
    
    // Enable/disable debug logging for this class
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = (AllowPrivateAccess = "true"))
    bool bEnableDebugLogging = true;

public:
    /**
     * Default constructor.
     * Initializes default values for this character's properties.
     */
    ASGCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
    /**
     * Called when the game starts or when spawned.
     * Use this for any initialization that needs to happen after the character is fully constructed.
     */
    virtual void BeginPlay() override;
    
    /**
     * Outputs debug information about the character's current state.
     * @param bForce - If true, will log even if debug logging is disabled
     */
    UFUNCTION(BlueprintCallable, Category = "Debug")
    virtual void DebugLogState(bool bForce = false) const;

    // Core Attributes
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", meta = (DisplayName = "Strength"))
    FSGAttributeData Strength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", meta = (DisplayName = "Dexterity"))
    FSGAttributeData Dexterity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", meta = (DisplayName = "Constitution"))
    FSGAttributeData Constitution;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", meta = (DisplayName = "Intelligence"))
    FSGAttributeData Intelligence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", meta = (DisplayName = "Wisdom"))
    FSGAttributeData Wisdom;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", meta = (DisplayName = "Charisma"))
    FSGAttributeData Charisma;

public:
    //~ Begin AActor Interface
    /**
     * Called every frame.
     * @param DeltaTime - The time since the last tick in seconds
     */
    virtual void Tick(float DeltaTime) override;

    /**
     * Called to bind functionality to input.
     * @param PlayerInputComponent - The input component to bind to
     */
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    //~ End AActor Interface

    /**
     * Gets the attribute data for the specified attribute type.
     * @param AttributeType - The type of attribute to retrieve
     * @return The attribute data structure
     */
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    FSGAttributeData GetAttribute(ESGAttributeType AttributeType) const;

    /**
     * Sets the base value of the specified attribute and recalculates its modifier.
     * @param AttributeType - The type of attribute to modify
     * @param NewValue - The new base value (will be clamped between 1-30)
     */
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void SetBaseAttribute(ESGAttributeType AttributeType, int32 NewValue);

    /**
     * Recalculates all attribute modifiers.
     * Call this after making multiple attribute changes for better performance.
     */
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void CalculateAllModifiers();

    /**
     * Gets the modifier for the specified attribute.
     * @param AttributeType - The type of attribute
     * @return The calculated modifier value
     */
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    int32 GetAttributeModifier(ESGAttributeType AttributeType) const;
    
    /**
     * Gets the current value of an attribute.
     * @param AttributeType - The type of attribute
     * @return The current base value
     */
    UFUNCTION(BlueprintPure, Category = "Attributes")
    int32 GetAttributeValue(ESGAttributeType AttributeType) const;
    
    /**
     * Helper function to get attribute display name as string.
     * @param AttributeType - The type of attribute
     * @return Display name as FString
     */
    UFUNCTION(BlueprintPure, Category = "Attributes")
    static FString GetAttributeName(ESGAttributeType AttributeType);
};
