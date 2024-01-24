// 2023 Jorge Sánchez, All rights reserved

#include "SPickupBase.h"

#include "Components/SphereComponent.h"

ASPickupBase::ASPickupBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(SphereComp);

	bIsActive = true;
	CooldownSeconds = 10.0f;
}

void ASPickupBase::Interact_Implementation(APawn* InstigatorPawn)
{
	if(!ensure(InstigatorPawn))
	{
		return;
	}
}

void ASPickupBase::EnablePickup()
{
	RootComponent->SetVisibility(true, true);
	SetActorEnableCollision(true);
	bIsActive = true;
}

void ASPickupBase::DisablePickup()
{
	RootComponent->SetVisibility(false, true);
	SetActorEnableCollision(false);
	bIsActive = false;
}

void ASPickupBase::BeginPlay()
{
	Super::BeginPlay();
}