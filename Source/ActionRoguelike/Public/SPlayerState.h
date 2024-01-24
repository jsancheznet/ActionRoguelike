// 2023 Jorge Sánchez, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	
	ASPlayerState();

	UFUNCTION(BlueprintCallable, Category = "Credits")
	UPARAM(DisplayName = "Credits") int GainCredits(int CreditAmount);
	
	UFUNCTION(BlueprintCallable, Category = "Credits")
	UPARAM(DisplayName = "Credits") int SpendCredits(int CreditAmount);
		
	UPROPERTY(BlueprintReadOnly, Category = "Credits")
	int PlayerCredits;
};