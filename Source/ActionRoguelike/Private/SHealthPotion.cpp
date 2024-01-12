// 2023 Jorge Sánchez, All rights reserved

#include "SHealthPotion.h"
#include "SAttributeComponent.h"

ASHealthPotion::ASHealthPotion()
{
	PrimaryActorTick.bCanEverTick = false;

	// TODO NOTE (Jsanchez): Tom added a SphereComponent and used that as root and collision!, in order to do that, create a sphere,
	// set it as root and set the mesh to Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision)
	// We can create the sphere in the base pickup class 
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = Mesh;

	HealingAmount = 20.0f;
	PotionCooldown = 10.0f;
	bIsActive = true;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if(!ensure(InstigatorPawn))
	{
		return;
	}
	
	// TODO(Jsanchez): Add Sound effect on consume
	USAttributeComponent *AttributeComp =  Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if(bIsActive && AttributeComp && !AttributeComp->IsFullHealth())
	{
		AttributeComp->ApplyHealthChange(HealingAmount);
		
		Disable();
		
		// Enable potion in PotionCooldown seconds
		GetWorldTimerManager().SetTimer(TimerHandle_Cooldown, this, &ASHealthPotion::Enable, PotionCooldown);		
	}
}

void ASHealthPotion::Disable()
{
	RootComponent->SetVisibility(false, false);
	SetActorEnableCollision(false);
	bIsActive = false;
}

void ASHealthPotion::Enable()
{
	RootComponent->SetVisibility(true, true);
	SetActorEnableCollision(true);
	bIsActive = true;	
}

void ASHealthPotion::BeginPlay()
{
	Super::BeginPlay();
}