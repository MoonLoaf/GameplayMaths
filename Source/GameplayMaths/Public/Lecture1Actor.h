#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lecture1Actor.generated.h"

UCLASS(hidecategories=(Input, Actor, Collision, Rendering, HLOD, WorldPartition, DataLayers, Replication, Physics, Networking, LevelInstance, Cooking))
class GAMEPLAYMATHS_API ALecture1Actor : public AActor
{
	GENERATED_BODY()
	
public:
	
	ALecture1Actor();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual bool ShouldTickIfViewportsOnly() const override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Base")
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Debug")
    bool DrawAveragePosition = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Context")
	TArray<AActor*> Demonstrators;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Context")
	float MaxDistance = 5000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Arc")
	bool DrawArc;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Arc")
	float Angle = 45;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Arc")
	float Range = 5000;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Intersection")
	float MeshSize = 250;

	bool bAreBoxesIntersecting;
	
	FVector MinBounds;
	FVector MaxBounds;

protected:

	UMaterialInstanceDynamic* DynamicMaterialInstance;

private:
	static TArray<AActor*> GetDemonstrators(UWorld* World);

	FVector CalculateAveragePosition(TArray<AActor*> Actors);
	
	bool AreBoxesIntersecting(
		const FVector& MinBounds1,
		const FVector& MaxBounds1,
		const FVector& MinBounds2,
		const FVector& MaxBounds2);
};
