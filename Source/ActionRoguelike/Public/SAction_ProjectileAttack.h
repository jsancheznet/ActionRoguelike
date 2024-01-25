// 2023 Jorge Sánchez, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage *AttackAnim;

	// Particle system played during attack animation
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem *CastingEffect;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter *InstigatorCharacter);

public:

	USAction_ProjectileAttack();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
};