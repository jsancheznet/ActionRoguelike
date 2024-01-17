// 2023 Jorge Sánchez, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class USAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ASAICharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent *PawnSensingComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components")
	USAttributeComponent *AttribComp;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnPawnSeen(APawn *Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	void SetTargetActor(AActor *NewTarget);
};