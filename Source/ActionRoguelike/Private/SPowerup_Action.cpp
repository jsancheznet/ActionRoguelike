// 2023 Jorge Sánchez, All rights reserved


#include "SPowerup_Action.h"

#include "SAction.h"
#include "SActionComponent.h"

void ASPowerup_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	// Make sure we have instigator & that action class was set up
	if(!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}

	USActionComponent *ActionComponent = Cast<USActionComponent>(InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass()));
	// Check if player already has action class
	if(ActionComponent)
	{
		if(ActionComponent->GetAction(ActionToGrant))
		{
			// UE_LOG(LogTemp, Log, TEXT("Instigator already has action of class: %s"), *GetNameSafe(ActionToGrant));
			FString DebugMsg = FString::Printf(TEXT("Action '%s' already known."), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *DebugMsg);
			return;
		}

		// Give New Ability
		ActionComponent->AddAction(InstigatorPawn, ActionToGrant);

		DisablePickup();
		GetWorldTimerManager().SetTimer(TimerHandle_Cooldown, this, &ASPowerup_Action::EnablePickup, CooldownSeconds);
	}
}