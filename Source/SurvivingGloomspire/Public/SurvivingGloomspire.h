// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * The main module class for the Surviving Gloomspire game module
 */
class FSurvivingGloomspireModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    
    /**
     * Singleton-like access to this module's interface.
     * @return Returns a singleton instance, loading the module on demand if needed
     */
    static inline FSurvivingGloomspireModule& Get()
    {
        return FModuleManager::LoadModuleChecked<FSurvivingGloomspireModule>("SurvivingGloomspire");
    }
    
    /**
     * Checks to see if this module is loaded and ready.
     * @return True if the module is loaded and ready to use
     */
    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("SurvivingGloomspire");
    }
};
