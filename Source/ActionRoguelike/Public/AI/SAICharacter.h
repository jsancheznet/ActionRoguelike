// 2023 Jorge Sánchez, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class USActionComponent;
class USWorldUserWidget;
class USAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ASAICharacter();

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int CreditWorthAmount;	

protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY()
	USWorldUserWidget *ActiveHealthBar;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent *PawnSensingComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components")
	USAttributeComponent *AttribComp;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent *ActionComp;

	/* Widget to display when bot first sees a player. */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass;

	/* Key for AI Blackboard 'TargetActor' */
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TargetActorKey;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnPawnSeen(APawn *Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);


	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTargetActor(AActor *NewTarget);

	UFUNCTION(BlueprintCallable, Category = "AI")	
	AActor *GetTargetActor() const;

	void SpawnSpottedWidget();
};