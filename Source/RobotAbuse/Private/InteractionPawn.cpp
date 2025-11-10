
// InteractionPawn.cpp
#include "InteractionPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "RobotPart.h"

AInteractionPawn::AInteractionPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

    PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
    PhysicsHandle->InterpolationSpeed = 10.f; // smooth following

    AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AInteractionPawn::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                Subsystem->ClearAllMappings();
                Subsystem->AddMappingContext(InteractionContext, 0);
            }
        }
        PlayerControllerRef = PlayerController;
        PlayerControllerRef->bShowMouseCursor = true;
        PlayerControllerRef->bEnableMouseOverEvents = true;
        PlayerControllerRef->bEnableClickEvents = true;
    }
}


// Initializing Enhanced Input Component
void AInteractionPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AInteractionPawn::IA_Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AInteractionPawn::IA_Look);
        EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Started, this, &AInteractionPawn::IA_LeftClick);
        EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AInteractionPawn::IA_LeftClick);
        EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Started, this, &AInteractionPawn::IA_RightClick);
        EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Completed, this, &AInteractionPawn::IA_RightClick);
    }
}


void AInteractionPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bIsInteracting)
        UpdateHeldObject();
}

void AInteractionPawn::IA_Move(const FInputActionValue& Value)
{
    const FVector2D Axis = Value.Get<FVector2D>();
    AddMovementInput(GetActorForwardVector(), Axis.Y);
    AddMovementInput(GetActorRightVector(), Axis.X);
}

void AInteractionPawn::IA_Look(const FInputActionValue& Value)
{
    const FVector2D LookAxis = Value.Get<FVector2D>();
    if (LookLock)
    {
        AddControllerYawInput(LookAxis.X);
        AddControllerPitchInput(LookAxis.Y);
    }
}

bool AInteractionPawn::TraceUnderCursor(FHitResult& OutHit) const
{
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (!PlayerController) return false;

    return PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, OutHit);
}    

void AInteractionPawn::IA_LeftClick(const FInputActionValue& Value)
{
    const bool bPressed = Value.Get<bool>();
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (!PlayerController || !PhysicsHandle) return;

    // Checks if LMB is down then checks to pick up Robot Part
    if (bPressed)
    {
        FHitResult Hit;
        if (TraceUnderCursor(Hit))
        {
            UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *Hit.GetActor()->GetName());
            if (ARobotPart* Part = Cast<ARobotPart>(Hit.GetActor()))
            {
                InteractingPart = Part;
                bIsInteracting = true;

                if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Hit.GetComponent()))
                {
                    UE_LOG(LogTemp, Warning, TEXT("Grabbing: %s"), *Part->GetName());
                    GrabOffset = Hit.Location - Prim->GetComponentLocation();
                    FRotator HitRot = Hit.GetActor()->GetActorRotation();
                    PhysicsHandle->GrabComponentAtLocationWithRotation(Hit.GetComponent(), NAME_None, Hit.Location, HitRot);
                    InteractingPart->OnPickedUp.Broadcast(true, InteractingPart->GetPartType(), InteractingPart); // enable hologram;
                }
            }
        }
    }
    // Checks if LMB is up then checks to drop Robot Part
    else
    {
        if (bIsInteracting && InteractingPart)
        {
            PhysicsHandle->ReleaseComponent();
            InteractingPart->TryAttach();
            InteractingPart->OnPickedUp.Broadcast(false, InteractingPart->GetPartType(), nullptr); // disable hologram
        }
        bIsInteracting= false;
        InteractingPart = nullptr;
    }
}


void AInteractionPawn::IA_RightClick(const FInputActionValue& Value)
{
    const bool bPressed = Value.Get<bool>();
    LookLock = bPressed;
    if (!PlayerControllerRef) return;

    PlayerControllerRef->bShowMouseCursor = !bPressed;
}

void AInteractionPawn::UpdateHeldObject() const
{
    if (!PhysicsHandle || !InteractingPart) return;
    if (!PlayerControllerRef) return;
    FVector WorldLoc, WorldDir;
    PlayerControllerRef->DeprojectMousePositionToWorld(WorldLoc, WorldDir);
    
    const FVector TargetLocation = WorldLoc + (WorldDir * GrabDistance);

    PhysicsHandle->SetTargetLocation(TargetLocation);
}
