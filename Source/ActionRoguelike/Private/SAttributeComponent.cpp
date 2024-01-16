// 2023 Jorge Sánchez, All rights reserved


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100.0f;
	Health = MaxHealth;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{

	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	float ActualDelta = Health - OldHealth;
	
	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);
	
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