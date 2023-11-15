#include "Lecture1Actor.h"

#include "Kismet/GameplayStatics.h"

ALecture1Actor::ALecture1Actor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);

	float rand;
	do {
		rand = FMath::RandRange(-600.f, 600.f);
	} while (FMath::IsWithinInclusive(rand, -200.f, 200.f));

	Velocity = FVector(rand, 0.f, 0.f);
}

void ALecture1Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALecture1Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FMath::Abs(GetActorLocation().X) >= 6000 || FMath::Abs(GetActorLocation().Y) >= 6000)
	{
		// Set actor's location to random XY values within -5000 and 5000
		SetActorLocation(FVector(
			FMath::RandRange(-5000.f, 5000.f),
			FMath::RandRange(-5000.f, 5000.f),
			GetActorLocation().Z
		));
	}

	if(CollisionCooldown > 0)
	{
		CollisionCooldown -= DeltaTime;
		CollisionCooldown = FMath::Max(CollisionCooldown, 0.0f);
	}

	if(bDrawXYZ)
	{
		const FVector Cross = FVector::CrossProduct(GetActorForwardVector(), GetActorRightVector());

		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 1000.f, FColor::Red);
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorRightVector() * 1000.f, FColor::Green);
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + Cross * 1000.f, FColor::Blue);
	}

	Demonstrators.Empty();
	Demonstrators = GetDemonstrators(GetWorld());

	/*
	 * Lecture 1
	 * Determine size based on distance to average position of all actors.
	 * Context being each actor position in relation to the group of actors
	 */
	FVector AveragePosition = CalculateAveragePosition(Demonstrators);
	
	//Debug average position of all demonstrators, only has to be true for one
	if(bDrawAveragePosition)
	{
		DrawDebugBox(GetWorld(), AveragePosition, FVector(100.f, 100.f, 100.f), FColor::Red, false, -1, -1, 10.f);
	}
	
	FVector Diff = GetActorLocation() - AveragePosition;

	float Distance = Diff.Size();
	float NormalizedDistance = FMath::Clamp(Distance / MaxDistance, 0.0f, 1.0f);

	ScaleFactor = FMath::Lerp(3.0f, 0.1f, NormalizedDistance);
	FVector ScaleVector = FVector(ScaleFactor, ScaleFactor, ScaleFactor);
	
	SetActorScale3D(ScaleVector);
	
	FVector MinPosition = FVector(FLT_MAX, FLT_MAX, FLT_MAX);
	FVector MaxPosition = FVector(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	/*
	 * Lecture 2
	 * Determine color based on each actors position in relation to all other actors
	 * Lerp Colors based on this information
	 */
	for (const AActor* Actor : Demonstrators)
	{
		MinPosition = FVector::Min(MinPosition, Actor->GetActorLocation());
		MaxPosition = FVector::Max(MaxPosition, Actor->GetActorLocation());
	}

	const FVector GridSize = MaxPosition - MinPosition;
	FVector NormalizedPosition = (GetActorLocation() - MinPosition) / GridSize;

	FLinearColor Color;

	Color.R = FMath::Lerp(0.f, 1.f, NormalizedPosition.X);
	Color.G = FMath::Lerp(0.f, 1.f, NormalizedPosition.Y);
	Color.B = 1.f - FMath::Lerp(0.f, 1.f, NormalizedPosition.X);

	FVector4 ColorVector = FVector4(Color.R, Color.G, Color.B, 1.f);
	
	/*
	 * Check for intersection
	 */

	if(ColliderType == ECollisionType::AABB)
	{
		FVector Center = GetActorLocation();
		MinBounds = Center - ScaleVector * 0.5f * MeshSize;
		MaxBounds = Center + ScaleVector * 0.5f * MeshSize;

		if(bDrawIntersection)
		{
			DrawDebugBox(GetWorld(), Center, ScaleVector * .5f * MeshSize, FColor::Orange, false, -1, -1, 10.f);
		}

		for (const AActor* OtherActor : Demonstrators)
		{
			if (OtherActor == this)
			{
				// Skip intersection check for the current actor
				continue;
			}
			const ALecture1Actor* other = Cast<ALecture1Actor>(OtherActor);

			if(other)
			{
				if(other->ColliderType == ECollisionType::AABB)
				{
					FVector MinBoundsOtherActor = other->MinBounds;
					FVector MaxBoundsOtherActor = other->MaxBounds;

					if (AreBoxesIntersecting(MinBounds, MaxBounds, MinBoundsOtherActor, MaxBoundsOtherActor))
					{
						ColorVector = FVector4(1.0f - ColorVector.X, 1.0f - ColorVector.Y, 1.0f - ColorVector.Z, 1.0f);
					}
				}
				else
				{
					FVector SphereCenterOtherActor = other->GetActorLocation();
					float SphereRadiusOtherActor = other->ScaleFactor *  0.5f * other->MeshSize;

					if(IsSphereAABBIntersecting(SphereCenterOtherActor, SphereRadiusOtherActor, MinBounds, MaxBounds))
					{
						ColorVector = FVector4(1.0f - ColorVector.X, 1.0f - ColorVector.Y, 1.0f - ColorVector.Z, 1.0f);
					}
				}
			}
		}
	}
	else
	{
		// Check for sphere-sphere and sphere-AABB collisions
		FVector SphereCenter = GetActorLocation();
		float SphereRadius = ScaleFactor * 0.5f * MeshSize;

		if(bDrawIntersection)
		{
			DrawDebugSphere(GetWorld(), SphereCenter, SphereRadius, 8,  FColor::Orange, false, -1, -1, 10.f);
		}

		for (const AActor* OtherActor : Demonstrators)
		{
			if (OtherActor == this)
			{
				// Skip intersection check for the current actor
				continue;
			}
			const ALecture1Actor* other = Cast<ALecture1Actor>(OtherActor);

			if(other)
			{
				if(other->ColliderType == ECollisionType::Sphere)
				{
					// Check for sphere-sphere intersection
					FVector SphereCenterOtherActor = other->GetActorLocation();
					float SphereRadiusOtherActor = other->ScaleFactor * 0.5f * other->MeshSize; // Assuming MeshSize is the diameter of the sphere

					if(AreSpheresIntersecting(SphereCenter, SphereRadius, SphereCenterOtherActor, SphereRadiusOtherActor))
					{
						ColorVector = FVector4(1.0f - ColorVector.X, 1.0f - ColorVector.Y, 1.0f - ColorVector.Z, 1.0f);
					}
				}
				else if(other->ColliderType == ECollisionType::AABB)
				{
					// Check for sphere-AABB intersection
					FVector MinBoundsOtherActor = other->MinBounds;
					FVector MaxBoundsOtherActor = other->MaxBounds;

					if(IsSphereAABBIntersecting(SphereCenter, SphereRadius, MinBoundsOtherActor, MaxBoundsOtherActor))
					{
						ColorVector = FVector4(1.0f - ColorVector.X, 1.0f - ColorVector.Y, 1.0f - ColorVector.Z, 1.0f);
					}
				}
			}
		}
	}
	
	
	//Set color based on information from intersection calculations and the positions relation to other actors
	DynamicMaterialInstance = StaticMeshComponent->CreateDynamicMaterialInstance(0);
    DynamicMaterialInstance->SetVectorParameterValue(TEXT("FinalColor"), ColorVector);

	AddActorWorldOffset(Velocity * DeltaTime);
	
	//Debug Arc
	if(!DrawArc)
		return;
	
	DrawDebugCone(
		GetWorld(),
		GetActorLocation(),
		GetActorForwardVector(),
		Range,
		FMath::DegreesToRadians(Angle * .5f),
		0.f,
		1,
		FColor::Green
		);
}

TArray<AActor*> ALecture1Actor::GetDemonstrators(UWorld* World)
{
	if (World)
	{
		TArray<AActor*> ActorInstances;
		UGameplayStatics::GetAllActorsOfClass(World, StaticClass(), ActorInstances);
		return ActorInstances;
	}
	return {};
}


FVector ALecture1Actor::CalculateAveragePosition(TArray<AActor*> Actors)
{
	FVector TotalPosition = FVector::ZeroVector;

	for (AActor* Actor : Actors)
	{
		if (Actor)
		{
			TotalPosition += Actor->GetActorLocation();
		}
	}

	if (Actors.Num() > 0)
	{
		TotalPosition /= Actors.Num();
	}

	return TotalPosition;
}

bool ALecture1Actor::AreBoxesIntersecting(const FVector& MinBounds1, const FVector& MaxBounds1, const FVector& MinBounds2, const FVector& MaxBounds2)
{
	// Check for intersection along the X-axis
	bool xIntersect = MaxBounds1.X >= MinBounds2.X && MinBounds1.X <= MaxBounds2.X;

	// Check for intersection along the Y-axis
	bool yIntersect = MaxBounds1.Y >= MinBounds2.Y && MinBounds1.Y <= MaxBounds2.Y;

	// Check for intersection along the Z-axis
	bool zIntersect = MaxBounds1.Z >= MinBounds2.Z && MinBounds1.Z <= MaxBounds2.Z;

	// If there is intersection along all axes, the boxes intersect
	return xIntersect && yIntersect && zIntersect;
}

bool ALecture1Actor::AreSpheresIntersecting(const FVector& Center1, float Radius1, const FVector& Center2, float Radius2)
{
	float DistanceSquared = FVector::DistSquared(Center1, Center2);
	float SumOfRadiiSquared = FMath::Square(Radius1 + Radius2);
	return DistanceSquared <= SumOfRadiiSquared;
}

bool ALecture1Actor::IsSphereAABBIntersecting(const FVector& SphereCenter, float SphereRadius, const FVector& AABBMinBounds, const FVector& AABBMaxBounds)
{
	FVector ClosestPointInAABB = FVector(
		FMath::Clamp(SphereCenter.X, AABBMinBounds.X, AABBMaxBounds.X),
		FMath::Clamp(SphereCenter.Y, AABBMinBounds.Y, AABBMaxBounds.Y),
		FMath::Clamp(SphereCenter.Z, AABBMinBounds.Z, AABBMaxBounds.Z)
	);

	float DistanceSquared = FVector::DistSquared(SphereCenter, ClosestPointInAABB);
	float RadiusSquared = FMath::Square(SphereRadius);

	return DistanceSquared <= RadiusSquared;
}

bool ALecture1Actor::ShouldTickIfViewportsOnly() const
{
	return true;
}



