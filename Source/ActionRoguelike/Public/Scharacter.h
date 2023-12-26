// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Scharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

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

	void MoveForward(float Value);
	void Turn(float Value);	
	
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};