#include "SGCharacterBase.h"

// Sets default values
ASGCharacterBase::ASGCharacterBase()
{
    // Set this character to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Initialize all attributes with default values
    Strength.BaseValue = 10;
    Dexterity.BaseValue = 10;
    Constitution.BaseValue = 10;
    Intelligence.BaseValue = 10;
    Wisdom.BaseValue = 10;
    Charisma.BaseValue = 10;

    // Calculate initial modifiers
    CalculateAllModifiers();
}

// Called when the game starts or when spawned
void ASGCharacterBase::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void ASGCharacterBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASGCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FAttributeData ASGCharacterBase::GetAttribute(EAttributeType AttributeType) const
{
    switch (AttributeType)
    {
    case EAttributeType::STR: return Strength;
    case EAttributeType::DEX: return Dexterity;
    case EAttributeType::CON: return Constitution;
    case EAttributeType::INT: return Intelligence;
    case EAttributeType::WIS: return Wisdom;
    case EAttributeType::CHA: return Charisma;
    default: return FAttributeData();
    }
}

void ASGCharacterBase::SetBaseAttribute(EAttributeType AttributeType, int32 NewValue)
{
    FAttributeData* AttributeToModify = nullptr;
    
    switch (AttributeType)
    {
    case EAttributeType::STR: AttributeToModify = &Strength; break;
    case EAttributeType::DEX: AttributeToModify = &Dexterity; break;
    case EAttributeType::CON: AttributeToModify = &Constitution; break;
    case EAttributeType::INT: AttributeToModify = &Intelligence; break;
    case EAttributeType::WIS: AttributeToModify = &Wisdom; break;
    case EAttributeType::CHA: AttributeToModify = &Charisma; break;
    default: return;
    }
    
    if (AttributeToModify)
    {
        AttributeToModify->BaseValue = FMath::Max(1, NewValue); // Ensure value is at least 1
        AttributeToModify->CalculateModifier();
    }
}

void ASGCharacterBase::CalculateAllModifiers()
{
    Strength.CalculateModifier();
    Dexterity.CalculateModifier();
    Constitution.CalculateModifier();
    Intelligence.CalculateModifier();
    Wisdom.CalculateModifier();
    Charisma.CalculateModifier();
}

int32 ASGCharacterBase::GetAttributeModifier(EAttributeType AttributeType) const
{
    switch (AttributeType)
    {
    case EAttributeType::STR: return Strength.Modifier;
    case EAttributeType::DEX: return Dexterity.Modifier;
    case EAttributeType::CON: return Constitution.Modifier;
    case EAttributeType::INT: return Intelligence.Modifier;
    case EAttributeType::WIS: return Wisdom.Modifier;
    case EAttributeType::CHA: return Charisma.Modifier;
    default: return 0;
    }
}
