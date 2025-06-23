# Data Management

## Overview
This document outlines the data management system for the Pathfinder 1e implementation, covering data assets, save/load functionality, and configuration management in Unreal Engine 5.

## Data Assets

### Creating Data Assets

#### Primary Data Assets
1. Right-click in Content Browser
2. Select "Miscellaneous > Data Asset"
3. Choose your data asset class (e.g., `SpellData`, `ItemData`)
4. Name and save the asset

#### Data Tables
1. Prepare a CSV or JSON file with your data
2. In Content Browser, right-click and select "Miscellaneous > Data Table"
3. Choose your row structure
4. Import your data file

### Data Asset Base Class
```cpp
UCLASS(Abstract, Blueprintable)
class MYGAME_API UGameDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()
    
public:
    // Unique identifier for this asset
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    FPrimaryAssetType DataType;
    
    // Get the primary asset ID
    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(DataType, GetFName());
    }
    
    // Get display name for UI
    UFUNCTION(BlueprintCallable, Category = "Data")
    virtual FText GetDisplayName() const { return FText::FromName(GetFName()); }
    
    // Get description for tooltips
    UFUNCTION(BlueprintCallable, Category = "Data")
    virtual FText GetDescription() const { return FText::GetEmpty(); }
};
```

## Save System

### Save Game Base Class
```cpp
UCLASS()
class MYGAME_API UMySaveGame : public USaveGame
{
    GENERATED_BODY()
    
public:
    // Save game version for compatibility
    UPROPERTY(VisibleAnywhere, Category = "Save")
    int32 SaveVersion;
    
    // Save timestamp
    UPROPERTY(VisibleAnywhere, Category = "Save")
    FDateTime SaveTime;
    
    // Save slot name
    UPROPERTY(VisibleAnywhere, Category = "Save")
    FString SaveSlotName;
    
    // User index
    UPROPERTY(VisibleAnywhere, Category = "Save")
    int32 UserIndex;
    
    // Character data
    UPROPERTY(VisibleAnywhere, Category = "Save")
    FCharacterSaveData CharacterData;
    
    // Level data
    UPROPERTY(VisibleAnywhere, Category = "Save")
    TMap<FName, FLevelSaveData> LevelData;
    
    // Game state data
    UPROPERTY(VisibleAnywhere, Category = "Save")
    TArray<uint8> GameStateData;
    
    // Initialize a new save
    void InitializeNewSave(const FString& InSlotName, int32 InUserIndex);
    
    // Serialize game state
    bool SerializeGameState(UObject* GameState);
    
    // Deserialize game state
    bool DeserializeGameState(UObject* GameState) const;
};
```

### Save Manager
```cpp
UCLASS()
class MYGAME_API USaveManager : public UObject
{
    GENERATED_BODY()
    
public:
    // Get the save manager instance
    UFUNCTION(BlueprintCallable, Category = "Save")
    static USaveManager* Get();
    
    // Save the current game
    UFUNCTION(BlueprintCallable, Category = "Save")
    bool SaveGame(const FString& SlotName, int32 UserIndex);
    
    // Load a saved game
    UFUNCTION(BlueprintCallable, Category = "Save")
    UMySaveGame* LoadGame(const FString& SlotName, int32 UserIndex);
    
    // Delete a saved game
    UFUNCTION(BlueprintCallable, Category = "Save")
    bool DeleteSave(const FString& SlotName, int32 UserIndex);
    
    // Get all save slots
    UFUNCTION(BlueprintCallable, Category = "Save")
    TArray<FString> GetSaveSlots() const;
    
    // Get save game info
    UFUNCTION(BlueprintCallable, Category = "Save")
    bool GetSaveInfo(const FString& SlotName, int32 UserIndex, FSaveGameInfo& OutInfo) const;
    
private:
    // Current save game
    UPROPERTY()
    UMySaveGame* CurrentSave;
    
    // Save directory
    FString SaveDirectory;
    
    // Initialize the save system
    void Initialize();
};
```

## Configuration Management

### Game Settings
```cpp
UCLASS(config=Game, defaultconfig)
class MYGAME_API UGameSettings : public UObject
{
    GENERATED_BODY()
    
public:
    UGameSettings(const FObjectInitializer& ObjectInitializer);
    
    // Get the game settings instance
    UFUNCTION(BlueprintCallable, Category = "Settings")
    static UGameSettings* Get();
    
    // Save settings to config
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void SaveSettings();
    
    // Video settings
    UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Video")
    FIntPoint Resolution;
    
    UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Video")
    int32 GraphicsQuality;
    
    UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Video")
    bool bFullscreen;
    
    // Audio settings
    UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float MasterVolume;
    
    UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float MusicVolume;
    
    UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float SFXVolume;
    
    // Game settings
    UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Game")
    bool bAutoSaveEnabled;
    
    UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Game")
    float AutoSaveInterval;
    
    // Input settings
    UPROPERTY(config)
    TArray<FInputActionKeyMapping> ActionMappings;
    
    UPROPERTY(config)
    TArray<FInputAxisKeyMapping> AxisMappings;
};
```

## Asset Management

### Asset Manager
```cpp
UCLASS()
class MYGAME_API UAssetManager : public UAssetManager
{
    GENERATED_BODY()
    
public:
    // Get the asset manager instance
    static UAssetManager& Get();
    
    // Load a primary asset
    template<typename T>
    static T* LoadAsset(const FPrimaryAssetId& AssetId, bool bLogError = true)
    {
        T* LoadedAsset = Cast<T>(Get().GetPrimaryAssetObject(AssetId));
        
        if (!LoadedAsset && bLogError)
        {
            UE_LOG(LogAssetManager, Error, TEXT("Failed to load asset: %s"), *AssetId.ToString());
        }
        
        return LoadedAsset;
    }
    
    // Load an asset asynchronously
    template<typename T>
    static FStreamableHandle* LoadAssetAsync(
        const FPrimaryAssetId& AssetId, 
        const FStreamableDelegate& OnLoaded, 
        bool bLogError = true)
    {
        TArray<FName> Bundles;
        Bundles.Add(FName("Game"));
        
        FStreamableManager& Streamable = Get().StreamableManager;
        
        return Streamable.RequestAsyncLoad(
            AssetId,
            FStreamableDelegate::CreateStatic(&OnAssetLoaded, OnLoaded, AssetId, bLogError),
            FStreamableManager::AsyncLoadHighPriority,
            false,
            Bundles);
    }
    
private:
    // Called when an asset is loaded
    static void OnAssetLoaded(FStreamableDelegate OnLoaded, FPrimaryAssetId AssetId, bool bLogError)
    {
        if (OnLoaded.IsBound())
        {
            OnLoaded.Execute();
        }
    }
};
```

## Localization

### Text Management
```cpp
// Define text namespaces
#define LOCTEXT_NAMESPACE "MyGame"

// Create localized text
FText MyText = LOCTEXT("HelloWorld", "Hello, World!");

// End namespace
#undef LOCTEXT_NAMESPACE
```

### Localization Setup
1. Create localization configuration files (.ini) in `Config/Localization/`
2. Set up culture directories (e.g., `Content/Localization/Game/en/`)
3. Use the Localization Dashboard to manage translations
4. Package with the desired cultures

## Performance Optimization

### Data Loading
1. Use async loading for non-critical assets
2. Implement asset streaming for large levels
3. Use asset bundles to group related assets

### Memory Management
1. Implement object pooling for frequently created/destroyed actors
2. Use soft references to avoid loading unused assets
3. Monitor memory usage with the Memory Profiler

## Best Practices

### Data Organization
1. Use a consistent folder structure
2. Prefix asset names with type (e.g., `DT_` for Data Tables, `DA_` for Data Assets)
3. Use subfolders to organize by feature or system

### Versioning
1. Include version numbers in save files
2. Implement upgrade paths for old save formats
3. Use the `SaveVersion` field to handle compatibility

### Error Handling
1. Validate data on load
2. Provide meaningful error messages
3. Handle missing or corrupt data gracefully

## Related Documents
- [Paper2D Setup](paper2d_setup.md)
- [Gameplay Ability System](gameplay_ability_system.md)
---
*Document Version: 1.0*  
*Last Updated: 2023-11-15*
