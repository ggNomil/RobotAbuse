// RobotPart.h
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "RobotPartTypes.h"
#include "RobotPart.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPickedUpSignature, bool, bPickedUp, ERobotPartType, PartType, ARobotPart*, RobotPart);


UCLASS()
class ARobotPart : public AActor
{
	GENERATED_BODY()

public:
	ARobotPart();
	
	virtual void TryAttach();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Part")
	UStaticMeshComponent* MeshRef;

	UPROPERTY(EditDefaultsOnly, Category="Part")
	bool bInteractable = false;

	UPROPERTY(BlueprintAssignable, Category="Robot Events")
	FOnPickedUpSignature OnPickedUp;

	// Allows me to keep PartType Protected
	UFUNCTION(BlueprintCallable)
	ERobotPartType GetPartType() const { return PartType; }
	
protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category="Robot")
	ERobotPartType PartType = ERobotPartType::None;
	
	bool bAttached = true;

};

