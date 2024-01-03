// Fill out your copyright notice in the Description page of Project Settings.

#include "SInteractionComponent.h"

#include "SGameplayInterface.h"
// #include "EntitySystem/MovieSceneEntitySystemRunner.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USInteractionComponent::PrimaryInteract()
{

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor *ComponentOwner = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotation;
	ComponentOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FVector End = EyeLocation + (EyeRotation.Vector()) * 1000;
	
	// FHitResult Hit = {};	
	// bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(30.0f);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor Color = bBlockingHit ? FColor::Green : FColor::Red;
	for(FHitResult Hit : Hits)
	{
		AActor *HitActor = Hit.GetActor();
		if(HitActor)
		{
			if(HitActor->Implements<USGameplayInterface>())
			{
				APawn *MyPawn = Cast<APawn>(ComponentOwner);
			
				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}

		float Radius = 30.0f;
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 16, Color, false, 2.0f);
	}

	
	DrawDebugLine(GetWorld(), EyeLocation, End, Color, false, 2.0, 4.0f);


}

// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}