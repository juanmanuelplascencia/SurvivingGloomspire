// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#include "SGCharacterClassData.h"

const FSGClassLevelData& USGCharacterClassData::GetLevelData(int32 Level) const
{
    static const FSGClassLevelData DefaultLevelData;
    if (Level > 0 && Level <= LevelData.Num())
    {
        return LevelData[Level - 1];
    }
    return DefaultLevelData;
}

FPrimaryAssetId USGCharacterClassData::GetPrimaryAssetId() const
{
    return FPrimaryAssetId("CharacterClass", GetFName());
}
