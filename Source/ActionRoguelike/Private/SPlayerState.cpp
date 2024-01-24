// 2023 Jorge Sánchez, All rights reserved

#include "SPlayerState.h"

ASPlayerState::ASPlayerState()
{
	PlayerCredits = 0;
}

int ASPlayerState::GainCredits(int CreditAmount)
{
	if(CreditAmount > 0)
	{
		PlayerCredits += CreditAmount;
	}
	
	return PlayerCredits;
}

int ASPlayerState::SpendCredits(int CreditAmount)
{
	if(CreditAmount > 0)
	{
		PlayerCredits -= CreditAmount;
		
		if(PlayerCredits < 0)
		{
			PlayerCredits = 0;
		}
	}
	
	return PlayerCredits;	
}