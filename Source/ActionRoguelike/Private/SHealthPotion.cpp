// 2023 Jorge Sánchez, All rights reserved

#include "SHealthPotion.h"
#include "SAttributeComponent.h"

ASHealthPotion::ASHealthPotion()
{
	PrimaryActorTick.bCanEverTick = false;
	
	HealingAmount = 20.0f;
	ItemCooldown = 10.0f;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if(!ensure(InstigatorPawn))
	{
		return;
	}
	
	// TODO(Jsanchez): Add Sound effect on consume
	USAttributeComponent *AttributeComp = USAttributeComponent::GetAttributes(InstigatorPawn);
	if(bIsActive && AttributeComp && !AttributeComp->IsFullHealth())
	{
		AttributeComp->ApplyHealthChange(this, HealingAmount);
		
		Disable();

		GetWorldTimerManager().SetTimer(TimerHandle_Cooldown, this, &ASHealthPotion::Enable, ItemCooldown);		
	}
}

void ASHealthPotion::BeginPlay()
{
	Super::BeginPlay();
}