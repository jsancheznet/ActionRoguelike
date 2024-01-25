// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Scharacter.generated.h"

class USActionComponent;
class USAttributeComponent;
class ASMagicProjectile;
class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API AScharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AScharacter();
	

protected:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent *CameraComp;
	
	UPROPERTY(VisibleAnywhere)	
	USpringArmComponent *SpringArmComp;

	UPROPERTY(EditAnywhere)
	USInteractionComponent *InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent *AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent *ActionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;
	
	void MoveForward(float Value);
	void MoveRight(float Value);

	void SprintStart();
	void SprintStop();
		
	void Turn(float Value);
	void LookUp(float Value);

	void Jump();

	void PrimaryAttack();
	void BlackHoleAttack();
	void DashAttack();
	
	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;
	
	virtual void BeginPlay() override;

	virtual FVector GetPawnViewLocation() const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);
};