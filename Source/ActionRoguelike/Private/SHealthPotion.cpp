// 2023 Jorge Sánchez, All rights reserved

#include "SHealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ASHealthPotion::ASHealthPotion()
{
	PrimaryActorTick.bCanEverTick = false;
	
	HealingAmount = 20.0f;
	CooldownSeconds = 10.0f;
	CreditCost = 30;	
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
	
	// TODO(Jsanchez): Add Sound effect on consume
	USAttributeComponent *AttributeComp = USAttributeComponent::GetAttributes(InstigatorPawn);
	if(bIsActive && AttributeComp && !AttributeComp->IsFullHealth())
	{
		ASPlayerState *PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
		if(!PlayerState)
		{
			return;
		}
		
		bool bPlayerHasEnoughCredits = PlayerState->GetCredits() >= CreditCost ? true : false;			
		if(bPlayerHasEnoughCredits)
		{
			AttributeComp->ApplyHealthChange(this, HealingAmount);
			DisablePickup();
			PlayerState->RemoveCredits(CreditCost);
			GetWorldTimerManager().SetTimer(TimerHandle_Cooldown, this, &ASHealthPotion::EnablePickup, CooldownSeconds);			
		}
	}
}

FText ASHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	USAttributeComponent *AttributeComp = USAttributeComponent::GetAttributes(InstigatorPawn);
	if(AttributeComp && AttributeComp->IsFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
	}


	return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum."), CreditCost);
}

void ASHealthPotion::BeginPlay()
{
	Super::BeginPlay();
}

#undef LOCTEXT_NAMESPACE