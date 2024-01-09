// 2023 Jorge Sánchez, All rights reserved

#include "SHealthPotion.h"
#include "SAttributeComponent.h"

ASHealthPotion::ASHealthPotion()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = Mesh;

	HealingAmount = 20.0f;
	PotionCooldown = 10.0f;
	bIsActive = true;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	// TODO(Jsanchez): Add Sound effect on consume
	USAttributeComponent *AttributeComp =  Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if(bIsActive && AttributeComp && !AttributeComp->IsFullHealth())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("NOT FULL HEALTH!"));
		
		AttributeComp->ApplyHealthChange(HealingAmount);
		
		Disable();
		
		// Enable potion in PotionCooldown seconds
		GetWorldTimerManager().SetTimer(TimerHandle_Cooldown, this, &ASHealthPotion::Enable, PotionCooldown);		
	}
}

void ASHealthPotion::Disable()
{
	Mesh->SetVisibility(false, false);
	bIsActive = false;
}

void ASHealthPotion::Enable()
{
	Mesh->SetVisibility(true, true);
	bIsActive = true;	
}

void ASHealthPotion::BeginPlay()
{
	Super::BeginPlay();
}