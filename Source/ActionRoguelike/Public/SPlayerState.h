// 2023 Jorge Sánchez, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreditsUpdated, int, CurrentCredits);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	
	ASPlayerState();

	UPROPERTY(BlueprintAssignable)
	FOnCreditsUpdated OnCreditsUpdated;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	UPARAM(DisplayName = "Credits") int GainCredits(int CreditAmount);
	
	UFUNCTION(BlueprintCallable, Category = "Credits")
	UPARAM(DisplayName = "Credits") int SpendCredits(int CreditAmount);
		
	UPROPERTY(BlueprintReadOnly, Category = "Credits")
	int CurrentCredits;
};