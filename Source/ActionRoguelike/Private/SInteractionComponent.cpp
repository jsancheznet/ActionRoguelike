// Fill out your copyright notice in the Description page of Project Settings.

#include "SInteractionComponent.h"

#include "SGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable debug lines for Interact Component"), ECVF_Cheat);

USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USInteractionComponent::PrimaryInteract()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor *ComponentOwner = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotation;
	ComponentOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FVector End = EyeLocation + (EyeRotation.Vector()) * 1000;

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(30.0f);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor Color = bBlockingHit ? FColor::Green : FColor::Red;
	for(FHitResult Hit : Hits)
	{
		if(bDebugDraw)
		{
			float Radius = 30.0f;
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 16, Color, false, 2.0f);			
		}
		
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
	}

	if(bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, Color, false, 2.0, 4.0f);		
	}
}

void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}