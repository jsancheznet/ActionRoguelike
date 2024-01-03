// 2023 Jorge Sánchez, All rights reserved


#include "STeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

ASTeleportProjectile::ASTeleportProjectile()
{
	TravelTimeBeforeExplosion = 2.0f;
	TravelTimeBeforeTeleport = 1.0f;
}

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &ASTeleportProjectile::Explode_TimeElapsed, TravelTimeBeforeExplosion);
}

void ASTeleportProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComp->OnComponentHit.AddDynamic(this, &ASTeleportProjectile::OnComponentHit);
}

void ASTeleportProjectile::Explode_TimeElapsed()
{
	Explode();
	
	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ASTeleportProjectile::Teleport_TimeElapsed, TravelTimeBeforeTeleport);	
}

void ASTeleportProjectile::Teleport_TimeElapsed()
{
	GetInstigator()->TeleportTo(GetActorLocation(), GetActorRotation(), false, false);
	Destroy();
}

void ASTeleportProjectile::Explode() const
{
	if(ensure(ExplodingParticleSystem))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodingParticleSystem, GetActorLocation(), GetActorRotation(), FVector(1.0f), true, EPSCPoolMethod::None, true);		
	}

	FHitResult Hit = {};
	MovementComp->StopSimulating(Hit);

	// EffectComp->DestroyComponent(false); // This seems to run a lot of more code than setvisibility 
	EffectComp->SetVisibility(false, false);
}

void ASTeleportProjectile::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	TimerHandle_Explode.Invalidate();
	Explode();
	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ASTeleportProjectile::Teleport_TimeElapsed, TravelTimeBeforeTeleport);	
}