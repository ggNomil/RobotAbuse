// RobotPartTypes.h
#pragma once

#include "RobotPartTypes.generated.h"

UENUM(BlueprintType)
enum class ERobotPartType : uint8
{
	None       UMETA(DisplayName = "None"),
	Head       UMETA(DisplayName = "Head"),
	ArmLeft    UMETA(DisplayName = "Arm Left"),
	ArmRight   UMETA(DisplayName = "Arm Right")
};
