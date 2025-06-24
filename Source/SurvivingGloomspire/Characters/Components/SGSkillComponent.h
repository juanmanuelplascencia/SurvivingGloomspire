// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SGSkillData.h"
#include "SGSkillComponent.generated.h"

class ASGCharacterBase;

/**
 * Component that manages a character's skills
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVINGGLOOMSPIRE_API USGSkillComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    USGSkillComponent();

    //~ Begin UActorComponent Interface
    virtual void BeginPlay() override;
    virtual void InitializeComponent() override;
    //~ End UActorComponent Interface

    /**
     * Initialize the skill component with the owner character
     */
    void Initialize(ASGCharacterBase* InOwnerCharacter);

    /**
     * Add ranks to a skill
     * @param SkillType The skill to add ranks to
     * @param RanksToAdd Number of ranks to add (can be negative to remove)
     * @return The new number of ranks in the skill
     */
    UFUNCTION(BlueprintCallable, Category = "Skills")
    int32 AddSkillRanks(ESGSkillType SkillType, int32 RanksToAdd);

    /**
     * Set whether a skill is a class skill
     */
    UFUNCTION(BlueprintCallable, Category = "Skills")
    void SetClassSkill(ESGSkillType SkillType, bool bIsClassSkill);

    /**
     * Get the total bonus for a skill, including all modifiers
     */
    UFUNCTION(BlueprintCallable, Category = "Skills")
    int32 GetSkillBonus(ESGSkillType SkillType) const;

    /**
     * Perform a skill check
     * @param SkillType The skill to check
     * @param DifficultyClass The DC to beat
     * @param Modifier Additional modifiers to the check
     * @return Tuple of (bSuccess, RollResult, DC)
     */
    UFUNCTION(BlueprintCallable, Category = "Skills")
    void PerformSkillCheck(ESGSkillType SkillType, int32 DifficultyClass, int32 Modifier, bool& bOutSuccess, int32& OutRollResult, int32& OutDC) const;

    /**
     * Get the number of ranks in a skill
     */
    UFUNCTION(BlueprintCallable, Category = "Skills")
    int32 GetSkillRanks(ESGSkillType SkillType) const;

    /**
     * Check if a skill is a class skill
     */
    UFUNCTION(BlueprintCallable, Category = "Skills")
    bool IsClassSkill(ESGSkillType SkillType) const;

    /**
     * Check if a skill can be used (not restricted by training requirements)
     */
    UFUNCTION(BlueprintCallable, Category = "Skills")
    bool CanUseSkill(ESGSkillType SkillType) const;

    /**
     * Get all skills as a map
     */
    const TMap<ESGSkillType, FSGSkillData>& GetAllSkills() const { return Skills.Skills; }

    /**
     * Get the key ability score for a skill
     */
    UFUNCTION(BlueprintCallable, Category = "Skills")
    static ESGAttributeType GetKeyAbilityForSkill(ESGSkillType SkillType);

    /**
     * Get the display name of a skill
     */
    UFUNCTION(BlueprintCallable, Category = "Skills")
    static FString GetSkillDisplayName(ESGSkillType SkillType);

protected:
    /** The character that owns this component */
    UPROPERTY()
    TWeakObjectPtr<ASGCharacterBase> OwnerCharacter;

    /** The character's skills */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills", meta = (AllowPrivateAccess = "true"))
    FSGSkillContainer Skills;

    /**
     * Get the relevant ability modifier for a skill
     */
    int32 GetAbilityModifierForSkill(ESGSkillType SkillType) const;

    /**
     * Get any miscellaneous modifiers that apply to a skill
     */
    int32 GetMiscModifiersForSkill(ESGSkillType SkillType) const;
};
