#include "Lecture1Actor.h"
#include "Constants.h"
#include "Kismet/GameplayStatics.h"

ALecture1Actor::ALecture1Actor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	//UMaterialInterface* BaseMaterial = StaticMeshComponent->GetMaterial(0);
	//DynamicMaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);
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

	FVector AveragePosition = CalculateAveragePosition(Demonstrators);
	FVector Diff = GetActorLocation() - AveragePosition;

	float Distance = Diff.Size();
	float NormalizedDistance = FMath::Clamp(Distance / MaxDistance, 0.0f, 1.0f);

	float ScaleFactor = FMath::Lerp(2.0f, 0.0f, NormalizedDistance);

	SetActorScale3D(FVector(ScaleFactor, ScaleFactor, ScaleFactor));

	//Calculate average rotation

	//float value = 10;

	//DynamicMaterialInstance->SetScalarParameterValue(TEXT("FinalColor"), value);
	//StaticMeshComponent->SetMaterial(0, DynamicMaterialInstance);


	if(!DrawArc)
		return;
	
	DrawDebugCone(
		GetWorld(),
		GetActorLocation(),
		GetActorForwardVector(),
		CONST_Range,
		FMath::DegreesToRadians(CONST_Angle * .5f),
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



