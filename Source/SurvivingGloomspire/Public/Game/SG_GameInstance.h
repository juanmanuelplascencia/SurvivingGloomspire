// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SG_GameInstance.generated.h"

/**
 * The main game instance class for Surviving Gloomspire
 */
UCLASS()
class SURVIVINGGLOOMSPIRE_API USG_GameInstance : public UGameInstance
{
    GENERATED_BODY()
    
public:
    USG_GameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
    
    //~ Begin UGameInstance Interface
    virtual void Init() override;
    virtual void Shutdown() override;
    //~ End UGameInstance Interface
    
protected:
    /** Called when the game starts or when spawned */
    virtual void OnStart() override;
    
    /** Initialize game systems */
    virtual void InitializeGameSystems();
    
    /** Shutdown game systems */
    virtual void ShutdownGameSystems();
};
