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
			AScharacter *PlayerCharacter = Cast<AScharacter>(InstigatorPawn);
			if(PlayerCharacter)
			{
				ASPlayerState *PlayerState = Cast<ASPlayerState>(PlayerCharacter->GetPlayerState());
				if(PlayerState)
				{
					PlayerState->GainCredits(CreditWorthAmount);
				}
			}
		}
		
		DisablePickup();
		GetWorldTimerManager().SetTimer(TimerHandle_Cooldown, this, &ASCoin::EnablePickup, CooldownSeconds);
	}
}