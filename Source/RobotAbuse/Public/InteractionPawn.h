// InteractionPawn.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "InteractionPawn.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class UPhysicsHandleComponent;
class ARobotPart;

UCLASS()
class AInteractionPawn : public APawn
{
	GENERATED_BODY()

public:
	AInteractionPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:

	// COMPONENTS
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;



	
	// INPUTS
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* InteractionContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* LeftClickAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* RightClickAction;

	UPROPERTY()
	APlayerController* PlayerControllerRef;
	
	bool bDragging = false;
	ARobotPart* SelectedPart = nullptr;

	// INPUT ACTIONS
	void IA_Move(const FInputActionValue& Value);
	void IA_Look(const FInputActionValue& Value);
	void IA_LeftClick(const FInputActionValue& Value);
	void IA_RightClick(const FInputActionValue& Value);

	void UpdateDrag(float DeltaTime);
	bool TraceUnderCursor(FHitResult& OutHit) const;

	// INTERACTION VARIABLES

	ARobotPart* InteractingPart = nullptr;
	FVector GrabOffset = FVector::ZeroVector; // not using
	bool LookLock = false;
	void UpdateHeldObject() const;


public:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UPhysicsHandleComponent* PhysicsHandle;
	
	UPROPERTY(BlueprintReadOnly, Category="Interaction")
	bool bIsInteracting = false;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Interaction")
	float GrabDistance = 150.0f;
};