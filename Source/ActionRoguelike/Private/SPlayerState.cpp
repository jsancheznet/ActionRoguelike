// 2023 Jorge Sánchez, All rights reserved

#include "SPlayerState.h"

#include "SSaveGame.h"
#include "Net/UnrealNetwork.h"

ASPlayerState::ASPlayerState()
{
	CurrentCredits = 0;
	bReplicates = true;
}

int ASPlayerState::AddCredits(int Delta)
{
	if(Delta > 0 && HasAuthority())
	{
		CurrentCredits += Delta;
		MulticastCreditsUpdated(this, CurrentCredits, Delta);		
	}
	
	return CurrentCredits;
}

int ASPlayerState::RemoveCredits(int Delta)
{
	if(Delta > 0 && HasAuthority())
	{
		CurrentCredits -= Delta;
		
		if(CurrentCredits < 0)
		{
			CurrentCredits = 0;
		}
		
		MulticastCreditsUpdated(this, CurrentCredits, Delta);
	}
	
	return CurrentCredits;	
}

int32 ASPlayerState::GetCredits() const
{
	return CurrentCredits;
}

void ASPlayerState::MulticastCreditsUpdated_Implementation(ASPlayerState* PlayerState, int32 Credits, int32 Delta)
{
	OnCreditsUpdated.Broadcast(PlayerState, Credits, Delta);
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
		AddCredits(SaveObject->Credits);
	}	
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASPlayerState, CurrentCredits);
}