// 2023 Jorge Sánchez, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "STeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASTeleportProjectile : public ASProjectileBase
{
	GENERATED_BODY()

	ASTeleportProjectile();

protected:

	UPROPERTY(EditAnywhere)
	UParticleSystem *ExplodingParticleSystem;

	UPROPERTY(EditAnywhere)
	float TravelTimeBeforeExplosion;
	
	UPROPERTY(EditAnywhere)
	float TravelTimeBeforeTeleport;	
	
	FTimerHandle TimerHandle_Explode;
	FTimerHandle TimerHandle_Teleport;

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	
	void Explode_TimeElapsed();
	void Teleport_TimeElapsed();

	void Explode() const;
	
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);	
};