#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ECollisionType : uint8
{
	None,
	Sphere,
	AABB,
};

ENUM_CLASS_FLAGS(ECollisionType);