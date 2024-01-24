// 2023 Jorge Sánchez, All rights reserved

#include "SHealthPotion.h"
#include "SAttributeComponent.h"
#include "Scharacter.h"
#include "SPlayerState.h"

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
		bool bPlayerHasEnoughCredits = false;
		ASPlayerState *PlayerState = nullptr;

		AScharacter *PlayerCharacter = Cast<AScharacter>(InstigatorPawn);
		if(PlayerCharacter)
		{
			PlayerState = Cast<ASPlayerState>(PlayerCharacter->GetPlayerState());
			if(PlayerState)
			{
				bPlayerHasEnoughCredits = PlayerState->PlayerCredits >= CreditCost ? true : false;
			}
		}
		
		if(bPlayerHasEnoughCredits)
		{
			AttributeComp->ApplyHealthChange(this, HealingAmount);
			DisablePickup();
			PlayerState->SpendCredits(CreditCost);
			GetWorldTimerManager().SetTimer(TimerHandle_Cooldown, this, &ASHealthPotion::EnablePickup, CooldownSeconds);			
		}
	}
}

void ASHealthPotion::BeginPlay()
{
	Super::BeginPlay();
}