// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Scharacter.generated.h"

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
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> MagicProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DashProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage *AttackAnim;

	/* Particle System played during attack animation */
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;	

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_BlackHoleAttack;
	FTimerHandle TimerHandle_DashAttack;
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	
	void Turn(float Value);
	void LookUp(float Value);

	void Jump();

	void PrimaryAttack();
	void BlackHoleAttack();
	void DashAttack();
	void PrimaryInteract();
	void PlayCastingSpellEffect();	

	void PrimaryAttack_TimeElapsed();
 	void BlackHoleAttack_TimeElapsed();
	void DashAttack_TimeElapsed();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;
	
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void SpawnProjectileClass(TSubclassOf<AActor> ProjectileClass);
	FVector GetProjectileTarget();
	
};