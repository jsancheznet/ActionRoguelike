// 2023 Jorge Sánchez, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "SPickupBase.h"
#include "SCoin.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASCoin : public ASPickupBase
{
	GENERATED_BODY()

public:

	ASCoin();

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int CreditWorthAmount;
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};