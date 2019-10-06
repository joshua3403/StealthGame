// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBalckHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFPSBalckHole::AFPSBalckHole()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	InnerComp = CreateAbstractDefaultSubobject<USphereComponent>(TEXT("InnerComp"));
	InnerComp->SetSphereRadius(100.0f);
	InnerComp->SetupAttachment(MeshComp);

	InnerComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSBalckHole::OnOverlapInnerComp);

	OutterComp = CreateAbstractDefaultSubobject<USphereComponent>(TEXT("OutterComp"));
	OutterComp->SetSphereRadius(3000.0f);
	OutterComp->SetupAttachment(MeshComp);
};

void AFPSBalckHole::OnOverlapInnerComp(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void AFPSBalckHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Find all overlapping components that can collide and may be physically simulating
	TArray<UPrimitiveComponent*> OverlappingComponents;
	OutterComp->GetOverlappingComponents(OverlappingComponents);


	for (int i = 0; i < OverlappingComponents.Num(); i++)
	{
		UPrimitiveComponent* PrimComp = OverlappingComponents[i];
		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			// The component we are looking for! It needs to be simulating in order to apply forces.

			const float SphereRadius = OutterComp->GetScaledSphereRadius();
			const float ForceStrength = -2000;	// Negative value to make it pull towards the origin instead of pushing away

			PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}

