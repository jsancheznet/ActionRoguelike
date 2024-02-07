// 2023 Jorge Sánchez, All rights reserved

#include "SPlayerState.h"

#include "SSaveGame.h"
#include "Net/UnrealNetwork.h"

ASPlayerState::ASPlayerState()
{
	CurrentCredits = 0;
	bReplicates = true;
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASPlayerState, CurrentCredits);
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

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	if(SaveObject)
	{
		SaveObject->Credits = CurrentCredits;
	}
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if(SaveObject)
	{
		CurrentCredits = SaveObject->Credits;
	}	
}