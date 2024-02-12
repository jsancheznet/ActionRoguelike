// 2023 Jorge Sánchez, All rights reserved

#include "SPickupBase.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

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

	bReplicates = true;
}

void ASPickupBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASPickupBase::Interact_Implementation(APawn* InstigatorPawn)
{
	if(!ensure(InstigatorPawn))
	{
		return;
	}
}

FText ASPickupBase::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void ASPickupBase::OnRep_bIsActive()
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive, true);
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

void ASPickupBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPickupBase, bIsActive);
}