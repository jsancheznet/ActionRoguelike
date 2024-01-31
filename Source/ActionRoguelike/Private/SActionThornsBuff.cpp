// 2023 Jorge Sánchez, All rights reserved


#include "SActionThornsBuff.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"

USActionThornsBuff::USActionThornsBuff()
{
	bAutoStart = true;

	Duration = 0.0f;
	Period = 0.0f;

	ReflectionMultiplier = 0.2f;
}

void USActionThornsBuff::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	// Bind to owner OnHealthChanged
	USAttributeComponent *AttributeComponent =  USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if(AttributeComponent)
	{
		AttributeComponent->OnHealthChanged.AddDynamic(this, &USActionThornsBuff::ReflectDamage);
	}
}

void USActionThornsBuff::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	// Unbind to OnHealthChanged
	USAttributeComponent *AttributeComponent =  USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if(AttributeComponent)
	{
		AttributeComponent->OnHealthChanged.RemoveDynamic(this, &USActionThornsBuff::ReflectDamage);
	}
}

void USActionThornsBuff::ReflectDamage(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor *OwningActor = GetOwningComponent()->GetOwner();

	// Damage only
	if(Delta < 0.0f && OwningActor != InstigatorActor)
	{
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectionMultiplier);
		if(ReflectedAmount == 0)
		{
			return;
		}
		
		USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, FMath::Abs(ReflectedAmount));
	}
}