// Copyright 2025 Surviving Gloomspire Team. All Rights Reserved.

#include "Player/SG_PlayerController.h"
#include "Player/SG_PlayerState.h"
#include "SG_LogChannels.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"

ASG_PlayerController::ASG_PlayerController()
{
    // Set default values
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableTouchEvents = true;
    bEnableMouseOverEvents = true;
    bInputComponentInitialized = false;
    
    // Set up input
    {
        static const FSoftObjectPath DefaultMappingContextPath(TEXT("/Game/Input/IMC_Default.IMC_Default"));
        if (UObject* LoadedObject = DefaultMappingContextPath.TryLoad())
        {
            DefaultMappingContext = Cast<UInputMappingContext>(LoadedObject);
        }
    }
    
    {
        ConstructorHelpers::FObjectFinder<UInputAction> MoveActionFinder(
            TEXT("/Game/Input/Actions/IA_Move"));
        if (MoveActionFinder.Succeeded())
        {
            MoveAction = MoveActionFinder.Object;
        }
    }
    
    {
        ConstructorHelpers::FObjectFinder<UInputAction> LookActionFinder(
            TEXT("/Game/Input/Actions/IA_Look"));
        if (LookActionFinder.Succeeded())
        {
            LookAction = LookActionFinder.Object;
        }
    }
    
    {
        ConstructorHelpers::FObjectFinder<UInputAction> JumpActionFinder(
            TEXT("/Game/Input/Actions/IA_Jump"));
        if (JumpActionFinder.Succeeded())
        {
            JumpAction = JumpActionFinder.Object;
        }
    }
    
    {
        ConstructorHelpers::FObjectFinder<UInputAction> CrouchActionFinder(
            TEXT("/Game/Input/Actions/IA_Crouch"));
        if (CrouchActionFinder.Succeeded())
        {
            CrouchAction = CrouchActionFinder.Object;
        }
    }
    
    {
        ConstructorHelpers::FObjectFinder<UInputAction> SprintActionFinder(
            TEXT("/Game/Input/Actions/IA_Sprint"));
        if (SprintActionFinder.Succeeded())
        {
            SprintAction = SprintActionFinder.Object;
        }
    }
    
    {
        ConstructorHelpers::FObjectFinder<UInputAction> PrimaryActionFinder(
            TEXT("/Game/Input/Actions/IA_PrimaryAction"));
        if (PrimaryActionFinder.Succeeded())
        {
            PrimaryAction = PrimaryActionFinder.Object;
        }
    }
    
    {
        ConstructorHelpers::FObjectFinder<UInputAction> SecondaryActionFinder(
            TEXT("/Game/Input/Actions/IA_SecondaryAction"));
        if (SecondaryActionFinder.Succeeded())
        {
            SecondaryAction = SecondaryActionFinder.Object;
        }
    }
    
    {
        ConstructorHelpers::FObjectFinder<UInputAction> InteractActionFinder(
            TEXT("/Game/Input/Actions/IA_Interact"));
        if (InteractActionFinder.Succeeded())
        {
            InteractAction = InteractActionFinder.Object;
        }
    }
    
    {
        ConstructorHelpers::FObjectFinder<UInputAction> ToggleMenuActionFinder(
            TEXT("/Game/Input/Actions/IA_ToggleMenu"));
        if (ToggleMenuActionFinder.Succeeded())
        {
            ToggleMenuAction = ToggleMenuActionFinder.Object;
        }
    }
}

void ASG_PlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize input mappings
    InitializeInputMappings();
    
    UE_LOG(LogSG_Game, Log, TEXT("SG_PlayerController: BeginPlay"));
}

void ASG_PlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
    
    // Update anything that needs to be updated every frame
}

void ASG_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // Movement
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASG_PlayerController::OnMove);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASG_PlayerController::OnLook);
        
        // Actions
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASG_PlayerController::OnJump);
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ASG_PlayerController::OnCrouch);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ASG_PlayerController::OnSprint);
        EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Triggered, this, &ASG_PlayerController::OnPrimaryAction);
        EnhancedInputComponent->BindAction(SecondaryAction, ETriggerEvent::Triggered, this, &ASG_PlayerController::OnSecondaryAction);
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ASG_PlayerController::OnInteract);
        EnhancedInputComponent->BindAction(ToggleMenuAction, ETriggerEvent::Triggered, this, &ASG_PlayerController::ToggleInGameMenu);
    }
    else
    {
        UE_LOG(LogSG_Game, Error, TEXT("SG_PlayerController: Failed to set up EnhancedInputComponent"));
    }
}

void ASG_PlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    
    UE_LOG(LogSG_Game, Log, TEXT("SG_PlayerController: Possessed pawn: %s"), 
        *GetNameSafe(InPawn));
}

void ASG_PlayerController::OnUnPossess()
{
    UE_LOG(LogSG_Game, Log, TEXT("SG_PlayerController: Unpossessed pawn"));
    
    Super::OnUnPossess();
}

void ASG_PlayerController::InitializeInputMappings()
{
    // Get the Enhanced Input subsystem
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        // Clear existing mappings
        Subsystem->ClearAllMappings();
        
        // Add the default mapping context if valid
        if (DefaultMappingContext)
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
            UE_LOG(LogSG_Game, Log, TEXT("SG_PlayerController: Added default input mapping context"));
        }
        else
        {
            UE_LOG(LogSG_Game, Warning, TEXT("SG_PlayerController: Default mapping context is null"));
        }
        
        // Setup input component if not already done
        if (!bInputComponentInitialized && InputComponent)
        {
            // Setup action bindings
            if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
            {
                // Bind the actions if they are valid
                if (MoveAction)
                {
                    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASG_PlayerController::OnMove);
                }
                
                if (LookAction)
                {
                    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASG_PlayerController::OnLook);
                }
                
                if (JumpAction)
                {
                    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASG_PlayerController::OnJump);
                }
                
                if (CrouchAction)
                {
                    EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ASG_PlayerController::OnCrouch);
                }
                
                if (SprintAction)
                {
                    EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ASG_PlayerController::OnSprint);
                    EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASG_PlayerController::OnSprint);
                }
                
                if (PrimaryAction)
                {
                    EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Triggered, this, &ASG_PlayerController::OnPrimaryAction);
                }
                
                if (SecondaryAction)
                {
                    EnhancedInputComponent->BindAction(SecondaryAction, ETriggerEvent::Triggered, this, &ASG_PlayerController::OnSecondaryAction);
                }
                
                if (InteractAction)
                {
                    EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ASG_PlayerController::OnInteract);
                }
                
                if (ToggleMenuAction)
                {
                    EnhancedInputComponent->BindAction(ToggleMenuAction, ETriggerEvent::Triggered, this, &ASG_PlayerController::ToggleInGameMenu);
                }
                
                bInputComponentInitialized = true;
                UE_LOG(LogSG_Game, Log, TEXT("SG_PlayerController: Initialized input bindings"));
            }
        }
    }
    else
    {
        UE_LOG(LogSG_Game, Error, TEXT("SG_PlayerController: Failed to get EnhancedInputSubsystem"));
    }
}

void ASG_PlayerController::OnPrimaryAction()
{
    UE_LOG(LogSG_Game, Verbose, TEXT("SG_PlayerController: Primary action triggered"));
    
    // Implement primary action logic
}

void ASG_PlayerController::OnSecondaryAction()
{
    UE_LOG(LogSG_Game, Verbose, TEXT("SG_PlayerController: Secondary action triggered"));
    
    // Implement secondary action logic
}

void ASG_PlayerController::OnMove(const FInputActionValue& Value)
{
    if (APawn* ControlledPawn = GetPawn())
    {
        const FVector2D MovementVector = Value.Get<FVector2D>();
        
        // Forward/backward direction
        if (MovementVector.Y != 0.0f)
        {
            ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), MovementVector.Y);
        }
        
        // Right/left direction
        if (MovementVector.X != 0.0f)
        {
            ControlledPawn->AddMovementInput(ControlledPawn->GetActorRightVector(), MovementVector.X);
        }
    }
}

void ASG_PlayerController::OnLook(const FInputActionValue& Value)
{
    if (APawn* ControlledPawn = GetPawn())
    {
        const FVector2D LookAxisVector = Value.Get<FVector2D>();
        
        // Add yaw and pitch input to controller
        if (LookAxisVector.X != 0.0f)
        {
            ControlledPawn->AddControllerYawInput(LookAxisVector.X);
        }
        
        if (LookAxisVector.Y != 0.0f)
        {
            ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
        }
    }
}

void ASG_PlayerController::OnJump()
{
    if (ACharacter* ControlledCharacter = GetCharacter())
    {
        ControlledCharacter->Jump();
    }
}

void ASG_PlayerController::OnCrouch()
{
    if (ACharacter* ControlledCharacter = GetCharacter())
    {
        if (ControlledCharacter->bIsCrouched)
        {
            ControlledCharacter->UnCrouch();
        }
        else
        {
            ControlledCharacter->Crouch();
        }
    }
}

void ASG_PlayerController::OnSprint()
{
    if (ACharacter* ControlledCharacter = GetCharacter())
    {
        // Handle sprint logic here
    }
}

void ASG_PlayerController::OnInteract()
{
    UE_LOG(LogSG_Game, Verbose, TEXT("SG_PlayerController: Interact action triggered"));
    
    // Implement interact logic
}

void ASG_PlayerController::ToggleInGameMenu()
{
    // HUD implementation not available
    UE_LOG(LogSG_Game, Verbose, TEXT("SG_PlayerController: ToggleInGameMenu called but HUD is not implemented"));
}
