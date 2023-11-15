#include "PlaneCollider.h"

#include "Lecture1Actor.h"
#include "Kismet/GameplayStatics.h"

APlaneCollider::APlaneCollider()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APlaneCollider::BeginPlay()
{
	Super::BeginPlay();
	
}

TArray<AActor*> APlaneCollider::GetDemonstrators(UWorld* World)
{
	if (World)
	{
		TArray<AActor*> Instances;
		UGameplayStatics::GetAllActorsOfClass(World, ALecture1Actor::StaticClass(), Instances);
		return Instances;
	}
	return {};
}

void APlaneCollider::HandleCollisions(TArray<AActor*> ArrDemonstrators, int32 CollisionMask)
{
	for (AActor* Actor : ArrDemonstrators)
	{
		ALecture1Actor* actor = Cast<ALecture1Actor>(Actor);

		if(actor->ColliderType == ECollisionType::Sphere && CollisionMask & static_cast<int32>(ECollisionType::Sphere))
		{	
			if(CheckPlaneSphereCollision(PlanePoints, actor->GetActorLocation(), actor-> ScaleFactor * 0.5f * actor->MeshSize))
			{
				if(actor->CollisionCooldown <= 0)
				{
					actor->CollisionCooldown = 1;
					actor->Velocity = -actor->Velocity;
				}
			}	
		}
		else if (actor->ColliderType == ECollisionType::AABB && CollisionBitmask & static_cast<int32>(ECollisionType::AABB))
		{
			if(CheckPlaneAABBCollision(PlanePoints, actor->MinBounds, actor->MaxBounds))
			{
				if(actor->CollisionCooldown <= 0)
				{
					actor->CollisionCooldown = 1;
					actor->Velocity = -actor->Velocity;
				}
			}
		}
	}
}

void APlaneCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Draw debug lines between each vector in PlanePoints
	DrawDebugLineBetweenVectors(PlanePoints.Point1, PlanePoints.Point2, FColor::Red);
	DrawDebugLineBetweenVectors(PlanePoints.Point2, PlanePoints.Point3, FColor::Green);
	DrawDebugLineBetweenVectors(PlanePoints.Point3, PlanePoints.Point4, FColor::Blue);
	DrawDebugLineBetweenVectors(PlanePoints.Point4, PlanePoints.Point1, FColor::Yellow);

	Demonstrators.Empty();
	Demonstrators = GetDemonstrators(GetWorld());

	HandleCollisions(Demonstrators, CollisionBitmask);
}

void APlaneCollider::DrawDebugLineBetweenVectors(const FVector& Start, const FVector& End, const FColor& Color)
{
	DrawDebugLine(GetWorld(), Start, End, Color, false, -1, -1, 10.f);
}

bool APlaneCollider::CheckPlaneSphereCollision(const FPlanePoints& planePoints, const FVector& sphereCenter, float sphereRadius)
{
	FVector normal = FVector::CrossProduct(planePoints.Point2 - planePoints.Point1, planePoints.Point3 - planePoints.Point1).GetSafeNormal();

	float distance = FVector::DotProduct(normal, sphereCenter - planePoints.Point1);

	return FMath::Abs(distance) <= sphereRadius;
}

bool APlaneCollider::CheckPlaneAABBCollision(const FPlanePoints& planePoints, const FVector& minBounds, const FVector& maxBounds)
{
	FVector normal = FVector::CrossProduct(planePoints.Point2 - planePoints.Point1, planePoints.Point3 - planePoints.Point1).GetSafeNormal();

	float distanceMin = FVector::DotProduct(normal, minBounds - planePoints.Point1);
	float distanceMax = FVector::DotProduct(normal, maxBounds - planePoints.Point1);

	return distanceMin * distanceMax < 0.0f;
}


bool APlaneCollider::ShouldTickIfViewportsOnly() const
{
	return true;
}

