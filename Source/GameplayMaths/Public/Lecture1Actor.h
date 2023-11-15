#pragma once

#include "CoreMinimal.h"
#include "CollisionTypes.h"
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
	FVector Velocity = FVector(100.f, 0.f, 0.f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Base")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Intersection")
	TEnumAsByte<ECollisionType> ColliderType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Debug")
    bool bDrawAveragePosition = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Debug")
	bool bDrawIntersection = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Debug")
	bool  bDrawXYZ = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Context")
	TArray<AActor*> Demonstrators;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Context")
	float MaxDistance = 7000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Arc")
	bool DrawArc;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Arc")
	float Angle = 45;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Arc")
	float Range = 5000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Intersection")
	float MeshSize; //Should be 250 for boxes, 320 for spheres. Also radius for sphere actors

	bool bAreBoxesIntersecting;

	float CollisionCooldown = 1;
	
	FVector MinBounds;
	FVector MaxBounds;

	float ScaleFactor;
	
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
	
	bool AreSpheresIntersecting(
		const FVector& Center1,
		float Radius1,
		const FVector& Center2,
		float Radius2);
	
	bool IsSphereAABBIntersecting(
		const FVector& SphereCenter,
		float SphereRadius,
		const FVector& AABBMinBounds,
		const FVector& AABBMaxBounds);
};
