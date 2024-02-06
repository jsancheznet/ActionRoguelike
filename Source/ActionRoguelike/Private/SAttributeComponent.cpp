// 2023 Jorge Sánchez, All rights reserved


#include "SAttributeComponent.h"

#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage modifier for Attribute Component"), ECVF_Cheat);

USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100.0f;
	Health = MaxHealth;

	Rage = 0.0f;
	MaxRage = 100.0f;
	RageMultiplier = 0.5f;

	SetIsReplicatedByDefault(true);
}


bool USAttributeComponent::Kill(AActor* Instigator)
{
	return ApplyHealthChange(Instigator, -GetMaxHealth());
}

bool USAttributeComponent::ApplyHealthChange(AActor *InstigatorActor, float Delta)
{
	if(!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if(Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	float ActualDelta = NewHealth - OldHealth;	
	if(GetOwner()->HasAuthority())
	{
		Health = NewHealth;
		if(ActualDelta != 0.0f)
		{
			MulticastHealthChange(InstigatorActor, Health, ActualDelta);		
		}

		// Died
		if(ActualDelta < 0.0f && Health == 0.0f)
		{
			ASGameModeBase *GameMode = GetWorld()->GetAuthGameMode<ASGameModeBase>();
			if(GameMode)
			{
				GameMode->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}
	
	return ActualDelta != 0;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0;
}

bool USAttributeComponent::IsFullHealth()
{
	return Health == MaxHealth;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

float USAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

bool USAttributeComponent::IsLowHealth() const
{
	ensureMsgf(LowHealthThreshold <= 1.0f && LowHealthThreshold >= 0.0f, TEXT("AttributeComponent LowHealthThreshold must be between 0.0f and 1.0f"));

	if(Health <= MaxHealth * LowHealthThreshold)
	{
		return true;
	}
	else
	{
		return false;
	}
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent *AttribComp =  GetAttributes(Actor);
	if(AttribComp)
	{
		return AttribComp->IsAlive();
	}

	return false;
}

int32 USAttributeComponent::GetRage() const
{
	return Rage;
}

bool USAttributeComponent::HasEnoughRage(int32 NeededRage)
{
	return Rage >= NeededRage;
}

bool USAttributeComponent::AddRageFromDamage(float Damage)
{
	int32 DeltaRage = FMath::RoundToInt(FMath::Abs(Damage * RageMultiplier));
	return AddRage(DeltaRage);
}

bool USAttributeComponent::AddRage(int32 Delta)
{
	int32 OldRage = Rage;
	Rage += Delta;
	Rage = FMath::Clamp(Rage, 0, MaxRage);

	if(Rage != OldRage)
	{
		OnRageChanged.Broadcast(this, Rage, Delta);
		return true;
	}

	return false;	
}

bool USAttributeComponent::RemoveRage(int32 Delta)
{
	float OldRage = Rage;
	Rage -= Delta;
	Rage = FMath::Clamp(Rage, 0, MaxRage);

	if(Rage != OldRage)
	{
		OnRageChanged.Broadcast(this, Rage, Delta);
		return true;
	}

	return false;
}

float USAttributeComponent::GetRageMultiplier()
{
	return RageMultiplier;
}

void USAttributeComponent::MulticastHealthChange_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, MaxHealth);
	// DOREPLIFETIME_CONDITION(USAttributeComponent, MaxHealth, COND_OwnerOnly);
}