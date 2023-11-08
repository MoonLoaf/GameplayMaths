#include "Lecture1Actor.h"
#include "Kismet/GameplayStatics.h"

ALecture1Actor::ALecture1Actor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
}

void ALecture1Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALecture1Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector Cross = FVector::CrossProduct(GetActorForwardVector(), GetActorRightVector());

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 1000.f, FColor::Red);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorRightVector() * 1000.f, FColor::Green);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + Cross * 1000.f, FColor::Blue);

	Demonstrators.Empty();
	Demonstrators = GetDemonstrators(GetWorld());


	/*
	// Determine size based on distance to average position of all actors.
	*/
	FVector AveragePosition = CalculateAveragePosition(Demonstrators);
	
	//Debug average position of all demonstrators, only has to be true for one
	if(DrawAveragePosition)
	{
		DrawDebugBox(GetWorld(), AveragePosition, FVector(100.f, 100.f, 100.f), FColor::Red, false, -1, -1, 10.f);
	}
	FVector Diff = GetActorLocation() - AveragePosition;

	float Distance = Diff.Size();
	float NormalizedDistance = FMath::Clamp(Distance / MaxDistance, 0.0f, 1.0f);

	float ScaleFactor = FMath::Lerp(2.0f, 0.0f, NormalizedDistance);

	SetActorScale3D(FVector(ScaleFactor, ScaleFactor, ScaleFactor));
	
	FVector MinPosition = FVector(FLT_MAX, FLT_MAX, FLT_MAX);
	FVector MaxPosition = FVector(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	/*
	// Determine color based on each actors position in relation to all other actors
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

	DynamicMaterialInstance = StaticMeshComponent->CreateDynamicMaterialInstance(0);
	DynamicMaterialInstance->SetVectorParameterValue(TEXT("FinalColor"), ColorVector);
	
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

bool ALecture1Actor::ShouldTickIfViewportsOnly() const
{
	return true;
}



