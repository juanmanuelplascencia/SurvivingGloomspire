// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SG_PlayerState.h"
#include "SG_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * Player controller for Surviving Gloomspire
 * Handles player input and UI
 */
UCLASS()
class SURVIVINGGLOOMSPIRE_API ASG_PlayerController : public APlayerController
{
    GENERATED_BODY()
    
public:
    ASG_PlayerController();
    
    //~ Begin APlayerController Interface
    virtual void PlayerTick(float DeltaTime) override;
    virtual void SetupInputComponent() override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;
    //~ End APlayerController Interface
    
    /** Get the player's team ID */
    UFUNCTION(BlueprintCallable, Category = "Player")
    int32 GetTeamId() const;
    
    /** Set the player's team ID */
    UFUNCTION(BlueprintCallable, Category = "Player")
    void SetTeamId(int32 NewTeamId);
    
    /** Handle the primary action (e.g., attack, interact) */
    UFUNCTION(BlueprintCallable, Category = "Input")
    void OnPrimaryAction();
    
    /** Handle the secondary action (e.g., aim, block) */
    UFUNCTION(BlueprintCallable, Category = "Input")
    void OnSecondaryAction();
    
    /** Handle movement input */
    UFUNCTION(BlueprintCallable, Category = "Input")
    void OnMove(const FInputActionValue& Value);
    
    /** Handle look input */
    UFUNCTION(BlueprintCallable, Category = "Input")
    void OnLook(const FInputActionValue& Value);
    
    /** Handle jump input */
    UFUNCTION(BlueprintCallable, Category = "Input")
    void OnJump();
    
    /** Handle crouch input */
    UFUNCTION(BlueprintCallable, Category = "Input")
    void OnCrouch();
    
    /** Handle sprint input */
    UFUNCTION(BlueprintCallable, Category = "Input")
    void OnSprint();
    
    /** Handle interact input */
    UFUNCTION(BlueprintCallable, Category = "Input")
    void OnInteract();
    
    /** Toggle the in-game menu */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ToggleInGameMenu();
    
protected:
    /** Called when the controller is ready to begin playing */
    virtual void BeginPlay() override;
    
    /** Initialize input mappings */
    virtual void InitializeInputMappings();
    
    /** The mapping context to use for player input */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> DefaultMappingContext;
    
    // Input actions
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> MoveAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> LookAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> JumpAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> CrouchAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> SprintAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> PrimaryAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> SecondaryAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> InteractAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> ToggleMenuAction;
    
    /** Whether the input component has been initialized */
    UPROPERTY(Transient)
    uint8 bInputComponentInitialized : 1;
};

// Inline function definitions
inline int32 ASG_PlayerController::GetTeamId() const
{
    if (!PlayerState)
    {
        return 0; // Default team
    }
    
    const ASG_PlayerState* SG_PS = Cast<ASG_PlayerState>(PlayerState);
    return SG_PS ? SG_PS->GetTeamId() : 0;
}

inline void ASG_PlayerController::SetTeamId(int32 NewTeamId)
{
    if (!PlayerState)
    {
        return;
    }
    
    ASG_PlayerState* SG_PS = Cast<ASG_PlayerState>(PlayerState);
    if (SG_PS)
    {
        SG_PS->SetTeamId(NewTeamId);
    }
}
