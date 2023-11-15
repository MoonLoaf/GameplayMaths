#pragma once

#define SET_BIT(Bitmask, Bit) (Bitmask |= 1 << static_cast<int32>(Bit))

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPlanePoints.h"
#include "PlaneCollider.generated.h"

UCLASS(hidecategories=(Input, Actor, Collision, Rendering, HLOD, WorldPartition, DataLayers, Replication, Physics, Networking, LevelInstance, Cooking))
class GAMEPLAYMATHS_API APlaneCollider : public AActor
{
	GENERATED_BODY()
	
public:	
	APlaneCollider();

	virtual void Tick(float DeltaTime) override;
	
	void DrawDebugLineBetweenVectors(const FVector& Start, const FVector& End, const FColor& Color);
	
	bool CheckPlaneSphereCollision(const FPlanePoints& planePoints, const FVector& sphereCenter, float sphereRadius);

	bool CheckPlaneAABBCollision(const FPlanePoints& planePoints, const FVector& minBounds, const FVector& maxBounds);

	virtual bool ShouldTickIfViewportsOnly() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (Bitmask,BitmaskEnum = "ECollisionType"))
    int32 CollisionBitmask;

	UPROPERTY(EditAnywhere, Category = "Plane points")
	FPlanePoints PlanePoints;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Context")
	TArray<AActor*> Demonstrators;

protected:

	virtual void BeginPlay() override;

private:
	static TArray<AActor*> GetDemonstrators(UWorld* World);

	void HandleCollisions(TArray<AActor*> ArrDemonstrators, int32 CollisionMask);

};
