// 2023 Jorge Sánchez, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPickupBase.generated.h"

class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API ASPickupBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ASPickupBase();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent *Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USphereComponent *SphereComp;

	FTimerHandle TimerHandle_Cooldown;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CooldownSeconds;	

	bool bIsActive;
	
	void EnablePickup();	
	void DisablePickup();
	
	virtual void BeginPlay() override;
};