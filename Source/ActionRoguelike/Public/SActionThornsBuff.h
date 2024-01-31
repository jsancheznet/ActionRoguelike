// 2023 Jorge Sánchez, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "SActionThornsBuff.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USActionThornsBuff : public USActionEffect
{
	GENERATED_BODY()

public:

	USActionThornsBuff();
	
	void StartAction_Implementation(AActor* Instigator) override;
	
	void StopAction_Implementation(AActor* Instigator) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float ReflectionMultiplier;

	UFUNCTION(BlueprintCallable, Category = "Thorns")
	void ReflectDamage(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
};