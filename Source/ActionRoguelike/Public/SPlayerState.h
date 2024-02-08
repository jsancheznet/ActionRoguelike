// 2023 Jorge Sánchez, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsUpdated, ASPlayerState*, PlayerState,  int32, CurrentCredits, int32, Delta);

class USSaveGame;

// TODO(Jsanchez): Animate the credits widget, a small pulse with color change 

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Credits")
	int CurrentCredits;

public:
	
	ASPlayerState();

	UPROPERTY(BlueprintAssignable)
	FOnCreditsUpdated OnCreditsUpdated;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	UPARAM(DisplayName = "Credits") int AddCredits(int CreditAmount);
	
	UFUNCTION(BlueprintCallable, Category = "Credits")
	UPARAM(DisplayName = "Credits") int RemoveCredits(int CreditAmount);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const;

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USSaveGame *SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USSaveGame *SaveObject);
};