// 2023 Jorge Sánchez, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRageChanged, USAttributeComponent*, OwningComp, int32, NewRage, int32, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USAttributeComponent();

protected:

	//
	// Health
	//
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float LowHealthThreshold;

	UFUNCTION(NetMulticast, Reliable) // @FIXME: Mark as unreliable once we moved the 'state' out of SCharacter
	void MulticastHealthChange(AActor *InstigatorActor, float NewHealth, float Delta);

	//
	// Rage
	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Rage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float MaxRage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float RageMultiplier;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRageChange(USAttributeComponent *OwningComp, int32 NewRage, int32 Delta);
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USAttributeComponent *GetAttributes(AActor *FromActor);

	//
	// Health
	//

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor *Instigator);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor *InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsFullHealth();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure)
	bool IsLowHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))	
	static bool IsActorAlive(AActor *Actor);

	//
	// Rage
	//

	// TODO(Jsanchez): Align the Health/Rage UMG boxes to always be the same size
	// TODO(Jsanchez): Put some text over the health and rage UMG bars indicating what each is for

	UPROPERTY(BlueprintAssignable);
	FOnRageChanged OnRageChanged;

	UFUNCTION(BlueprintCallable, BlueprintPure,  Category = "Rage")
	int32 GetRage() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = "Rage")
	bool HasEnoughRage(int32 NeededRage);

	UFUNCTION(BlueprintCallable, Category = "Rage")
	bool AddRageFromDamage(float Damage);	

	UFUNCTION(BlueprintCallable,  Category = "Rage")
	bool AddRage(int32 Delta);
	
	UFUNCTION(BlueprintCallable,  Category = "Rage")
	bool RemoveRage(int32 Delta);	

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Rage")
	float GetRageMultiplier();
};