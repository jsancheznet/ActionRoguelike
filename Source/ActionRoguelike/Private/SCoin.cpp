// 2023 Jorge Sánchez, All rights reserved


#include "SCoin.h"

#include "Scharacter.h"
#include "SPlayerState.h"

ASCoin::ASCoin()
{
	CreditWorthAmount = 100;
}

void ASCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	if(bIsActive)
	{
		{ // Give the interacting player credits
			ASPlayerState *PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
			if(PlayerState)
			{
				PlayerState->AddCredits(CreditWorthAmount);
			}
		}
		
		DisablePickup();
		GetWorldTimerManager().SetTimer(TimerHandle_Cooldown, this, &ASCoin::EnablePickup, CooldownSeconds);
	}
}