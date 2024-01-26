// 2023 Jorge Sánchez, All rights reserved

#include "SPlayerState.h"

ASPlayerState::ASPlayerState()
{
	CurrentCredits = 0;
}

int ASPlayerState::AddCredits(int CreditAmount)
{
	if(CreditAmount > 0)
	{
		CurrentCredits += CreditAmount;
		OnCreditsUpdated.Broadcast(this, CurrentCredits, CreditAmount);		
	}
	
	return CurrentCredits;
}

int ASPlayerState::RemoveCredits(int CreditAmount)
{
	if(CreditAmount > 0)
	{
		CurrentCredits -= CreditAmount;
		
		if(CurrentCredits < 0)
		{
			CurrentCredits = 0;
		}

		OnCreditsUpdated.Broadcast(this, CurrentCredits, CreditAmount);		
	}
	
	return CurrentCredits;	
}

int32 ASPlayerState::GetCredits() const
{
	return CurrentCredits;
}