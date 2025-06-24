#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SGCharacterBase.generated.h"

// Enum for core attributes
UENUM(BlueprintType)
enum class EAttributeType : uint8
{
    STR,  // Strength
    DEX,  // Dexterity
    CON,  // Constitution
    INT,  // Intelligence
    WIS,  // Wisdom
    CHA   // Charisma
};

// Structure to hold attribute data
USTRUCT(BlueprintType)
struct FAttributeData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    int32 BaseValue;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
    int32 Modifier;

    FAttributeData() : BaseValue(10), Modifier(0) {}

    // Calculate modifier based on attribute value (Pathfinder rules: (value - 10) / 2)
    void CalculateModifier()
    {
        Modifier = FMath::FloorToInt((BaseValue - 10) / 2.0f);
    }
};

UCLASS(Blueprintable)
class SURVIVINGGLOOMSPIRE_API ASGCharacterBase : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ASGCharacterBase();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Core Attributes
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FAttributeData Strength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FAttributeData Dexterity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FAttributeData Constitution;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FAttributeData Intelligence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FAttributeData Wisdom;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    FAttributeData Charisma;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Get attribute by type
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    FAttributeData GetAttribute(EAttributeType AttributeType) const;

    // Set base attribute value
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void SetBaseAttribute(EAttributeType AttributeType, int32 NewValue);

    // Calculate all attribute modifiers
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    void CalculateAllModifiers();

    // Get attribute modifier
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    int32 GetAttributeModifier(EAttributeType AttributeType) const;
};
