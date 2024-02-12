// 2023 Jorge Sánchez, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "SPickupBase.h"
#include "GameFramework/Actor.h"
#include "SHealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASPickupBase
{
	GENERATED_BODY()
	
public:	
	ASHealthPotion();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HealingAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int CreditCost;	

	virtual void BeginPlay() override;
};