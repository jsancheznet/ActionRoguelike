// 2023 Jorge Sánchez, All rights reserved

#include "SGameplayFunctionLibrary.h"

#include "SAttributeComponent.h"

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	USAttributeComponent *AttributeComponent = USAttributeComponent::GetAttributes(TargetActor);
	if(AttributeComponent)
	{
		return AttributeComponent->ApplyHealthChange(DamageCauser, -DamageAmount);
	}

	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if(ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent *HitComp = HitResult.GetComponent();
		if(HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			// Direction = Target - Origin
			
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();
			
			HitComp->AddImpulseAtLocation(Direction * 300000.0f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		
		return true;
	}

	return false;
}