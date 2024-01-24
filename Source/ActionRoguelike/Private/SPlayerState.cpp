// 2023 Jorge Sánchez, All rights reserved

#include "SPlayerState.h"

ASPlayerState::ASPlayerState()
{
	CurrentCredits = 0;
}

int ASPlayerState::GainCredits(int CreditAmount)
{
	if(CreditAmount > 0)
	{
		CurrentCredits += CreditAmount;
		OnCreditsUpdated.Broadcast(CurrentCredits);		
	}
	
	return CurrentCredits;
}

int ASPlayerState::SpendCredits(int CreditAmount)
{
	if(CreditAmount > 0)
	{
		CurrentCredits -= CreditAmount;
		
		if(CurrentCredits < 0)
		{
			CurrentCredits = 0;
		}

		OnCreditsUpdated.Broadcast(CurrentCredits);
	}
	
	return CurrentCredits;	
}