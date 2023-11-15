#pragma once

#include "CoreMinimal.h"
#include "FPlanePoints.generated.h"

USTRUCT(BlueprintType)
struct GAMEPLAYMATHS_API FPlanePoints
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
	FVector Point1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
	FVector Point2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
	FVector Point3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
	FVector Point4;
};