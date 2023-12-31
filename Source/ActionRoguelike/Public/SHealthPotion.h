// 2023 Jorge Sánchez, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SHealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

	//
	// TODO(Jsanchez): Once multiple pickups are implemented, abstract common functionality into a BasePickup
	// 
	
public:	
	ASHealthPotion();

	void Interact_Implementation(APawn* InstigatorPawn);

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent *Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HealingAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PotionCooldown;	

	bool bIsActive;
	FTimerHandle TimerHandle_Cooldown;

	void Disable();
	void Enable();
	
	virtual void BeginPlay() override;
};