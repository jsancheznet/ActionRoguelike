// 2023 Jorge Sánchez, All rights reserved


#include "SActionThornsBuff.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"

USActionThornsBuff::USActionThornsBuff()
{
	bAutoStart = true;

	Duration = 0.0f;
	Period = 0.0f;

	DamageReflectedMultiplier = 0.2f;
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

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("THORNS: START_ACTION"));
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

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("THORNS: STOP_ACTION"));	
}

void USActionThornsBuff::ReflectDamage(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	USAttributeComponent *InstigatorAttribComp = USAttributeComponent::GetAttributes(InstigatorActor);
	if(ensure(InstigatorAttribComp))
	{
		float DamageReflected = FMath::RoundToFloat(FMath::Abs(Delta) * DamageReflectedMultiplier);
		if(FMath::Abs(Delta) > 0 && DamageReflected == 0)
		{
			DamageReflected = 1.0f;
		}

		InstigatorAttribComp->ApplyHealthChange(GetOwningComponent()->GetOwner(), -DamageReflected);

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("THORNS: DAMAGE REFLECTED SUCCESFULLY"));
	}
}